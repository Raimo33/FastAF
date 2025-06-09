/*================================================================================

File: MarketDataClient.cpp                                                         
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-09 20:07:26                                                

================================================================================*/

#include "MarketDataClient.hpp"
#include "messages/SBEMessages.hpp"
#include "messages/InternalMessages.hpp"
#include "macros.hpp"

//logging with individual SPSC queues (not IPC, using threads) (each message is a span object)

COLD MarketDataClient::MarketDataClient(const currency_pair &pair, std::string_view api_key) :
  _pair(pair),
  _api_key(api_key),
  _ssl_ctx(ssl::context::tlsv13_client),
  _resolver(_io_ctx),
  _ws_stream(_io_ctx, _ssl_ctx),
  _read_buffer(1024),
  _mem_name("/binance_" + _pair.first + _pair.second + "_queue"),
  _queue_fd(shm_open(_mem_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)),
  _queue(_queue_fd)
{
  _ssl_ctx.set_default_verify_paths();
  _ssl_ctx.set_verify_mode(ssl::verify_peer);
}

COLD MarketDataClient::~MarketDataClient()
{
  _io_ctx.stop();
  close(_queue_fd);
  shm_unlink(_mem_name.c_str());
}

COLD void MarketDataClient::start(void) noexcept
{
  _resolver.async_resolve(HOSTNAME, PORT,
    [this](const boost::system::error_code &ec, const tcp::resolver::results_type &results) {
      onResolve(ec, results);
    });

  _io_ctx.run();
}

COLD void MarketDataClient::onResolve(const beast::error_code &ec, const tcp::resolver::results_type &results)
{
  assert(!ec);

  auto &stream = beast::get_lowest_layer(_ws_stream);

  stream.async_connect(results,
    [this](const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint) {
      onConnect(ec, endpoint);
    });
}

COLD void MarketDataClient::onConnect(const beast::error_code &ec, UNUSED const tcp::resolver::results_type::endpoint_type &endpoint)
{
  assert(!ec);

  auto &stream = _ws_stream.next_layer();
  auto &socket = beast::get_lowest_layer(stream).socket();

  socket.set_option(tcp::no_delay(true));

  stream.async_handshake(ssl::stream_base::client,
    [this](const beast::error_code &ec) {
      onSSLHandshake(ec);
    });
}

COLD void MarketDataClient::onSSLHandshake(const beast::error_code &ec)
{
  assert(!ec);

  _ws_stream.set_option(websocket::stream_base::decorator(
    [this](websocket::request_type &req) {
      req.set("X-MBX-APIKEY", _api_key);
    }));

  const std::string path = "/ws/" + _pair.first + _pair.second + "@bestBidAsk";

  _ws_stream.async_handshake(HOSTNAME, path,
    [this](const beast::error_code &ec) {
      onWSHandshake(ec);
    });
}

COLD void MarketDataClient::onWSHandshake(const beast::error_code &ec)
{
  assert(!ec);

  _ws_stream.control_callback(
  [this](websocket::frame_type kind, std::string &&payload) {
    onControl(kind, std::move(payload));
  });

  _ws_stream.binary(true);

  _ws_stream.async_read(_read_buffer,
    [this](const beast::error_code &ec, size_t UNUSED bytes_transferred) {
      onFirstRead(ec);
    });
}

COLD void MarketDataClient::onFirstRead(const beast::error_code &ec)
{
  assert(!ec);
  assert(_ws_stream.got_binary());

  std::span<const std::byte> data = getSpan(_read_buffer);

  const auto &message = *reinterpret_cast<const SBEMessage *>(data.data());
  assert(message.header.template_id == 10001);
  const auto &event = message.best_bid_ask_stream_event;

  _price_exponent = event.price_exponent;
  _qty_exponent = event.qty_exponent;

  InternalMessage m;
  std::strncpy(m.pair_info.base_currency, _pair.first.data(), 8);
  std::strncpy(m.pair_info.quote_currency, _pair.second.data(), 8);
  m.pair_info.price_exponent = _price_exponent;
  m.pair_info.qty_exponent = _qty_exponent;

  _queue.push(m);
  asyncRead();
}

HOT void MarketDataClient::onRead(const beast::error_code &ec)
{
  assert(!ec);
  assert(_ws_stream.got_binary());

  std::span<const std::byte> data = getSpan(_read_buffer);

  const auto &message = *reinterpret_cast<const SBEMessage *>(data.data());
  const auto &header = message.header;
  const auto &event = message.best_bid_ask_stream_event;

  assert(
    (header.template_id == 10001) &
    (event.price_exponent == _price_exponent) &
    (event.qty_exponent == _qty_exponent)
  );

  _queue.push(InternalMessage{
    .top_of_book = {
      event.bid_price,
      event.bid_qty,
      event.ask_price,
      event.ask_qty,
    }
  });

  _read_buffer.consume(data.size());
  asyncRead();
}

HOT void MarketDataClient::onControl(websocket::frame_type kind, std::string &&payload)
{
  switch (kind)
  {
    case websocket::frame_type::ping:
      onPing(std::move(payload));
      break;
    case websocket::frame_type::close:
      onClose();
      break;
    default:
      break;
  }
}

HOT void MarketDataClient::onPing(std::string &&payload)
{
  websocket::ping_data pong_payload(std::move(payload));

  _ws_stream.async_pong(pong_payload,
    [this](const beast::error_code &ec) {
      onPong(ec);
    });
}

HOT void MarketDataClient::onPong(const beast::error_code &ec)
{
  assert(!ec);
}

COLD void MarketDataClient::onClose(void)
{
  _io_ctx.stop();
}

HOT std::span<const std::byte> MarketDataClient::getSpan(const beast::flat_buffer &buffer)
{
  auto buffers = buffer.data();
  const std::byte *ptr = static_cast<const std::byte *>(boost::asio::buffer_cast<const void*>(buffers));
  const size_t size = boost::asio::buffer_size(buffers);

  return {ptr, size};
}

HOT void MarketDataClient::asyncRead(void)
{
  _ws_stream.async_read(_read_buffer,
    [this](const beast::error_code &ec, UNUSED size_t bytes_transferred) {
      onRead(ec);
    });
}
