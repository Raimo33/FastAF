/*================================================================================

File: BinanceClient.cpp                                                         
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#include "BinanceClient.hpp"
#include "messages/BinanceMessages.hpp"
#include "messages/InternalMessages.hpp"
#include "macros.hpp"

//logging with individual SPSC queues (not IPC, using threads) (each message is a span object)

COLD BinanceClient::BinanceClient(const currency_pair &pair, std::string_view api_key) :
  _pair(pair),
  _api_key(api_key),
  _price_exponent(0),
  _qty_exponent(0),
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

COLD BinanceClient::~BinanceClient()
{
  close(_queue_fd);
  shm_unlink(_mem_name.c_str());
}

COLD void BinanceClient::start(void) noexcept
{
  _resolver.async_resolve(HOSTNAME, PORT,
    [this](const boost::system::error_code &ec, const tcp::resolver::results_type &results) {
      onResolve(ec, results);
    });

  _io_ctx.run();
}

COLD void BinanceClient::onResolve(const beast::error_code &ec, const tcp::resolver::results_type &results)
{
  assert(!ec);

  auto &stream = beast::get_lowest_layer(_ws_stream);

  stream.async_connect(results,
    [this](const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint) {
      onConnect(ec, endpoint);
    });
}

COLD void BinanceClient::onConnect(const beast::error_code &ec, UNUSED const tcp::resolver::results_type::endpoint_type &endpoint)
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

COLD void BinanceClient::onSSLHandshake(const beast::error_code &ec)
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

COLD void BinanceClient::onWSHandshake(const beast::error_code &ec)
{
  assert(!ec);

  _ws_stream.control_callback(
  [this](websocket::frame_type kind, std::string &&payload) {
    onControl(kind, std::move(payload));
  });

  _ws_stream.binary(true);

  _ws_stream.async_read(_read_buffer,
    [this](const beast::error_code &ec, size_t UNUSED bytes_transferred) {
      onRead(ec);
    });
}

HOT void BinanceClient::onRead(const beast::error_code &ec)
{
  assert(!ec);
  assert(_ws_stream.got_binary());

  auto buffers = _read_buffer.data();
  const std::byte *ptr = static_cast<const std::byte *>(boost::asio::buffer_cast<const void*>(buffers));
  const size_t size = boost::asio::buffer_size(buffers);

  processData({ptr, size});
  _read_buffer.consume(size);
  
  _ws_stream.async_read(_read_buffer,
    [this](const beast::error_code &ec, UNUSED size_t bytes_transferred) {
      onRead(ec);
    });
}

HOT void BinanceClient::onControl(websocket::frame_type kind, std::string &&payload)
{
  switch (kind)
  {
    case websocket::frame_type::ping:
      onPing(std::move(payload));
      break;
    case websocket::frame_type::close:
      //handle close
      break;
    default:
      break;
  }
}

HOT void BinanceClient::onPing(std::string &&payload)
{
  websocket::ping_data pong_payload(std::move(payload));

  _ws_stream.async_pong(pong_payload,
    [this](const beast::error_code &ec) {
      onPong(ec);
    });
}

HOT void BinanceClient::onPong(const beast::error_code &ec)
{
  assert(!ec);
}

HOT void BinanceClient::processData(std::span<const std::byte> data)
{
  using namespace messages::binance::sbe;
  using namespace messages::internal;

  const Header *header = reinterpret_cast<const Header *>(data.data());
  assert(header->template_id == 10001);

  data = data.subspan(sizeof(Header));

  const BestBidAskStreamEvent *event = reinterpret_cast<const BestBidAskStreamEvent *>(data.data());

  _queue.push(TopOfBook{
    event->bid_price,
    event->bid_qty,
    event->ask_price,
    event->ask_qty,
    event->price_exponent,
    event->qty_exponent
  });
}
