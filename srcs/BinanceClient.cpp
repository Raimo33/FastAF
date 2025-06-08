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

COLD BinanceClient::BinanceClient(std::string_view base, std::string_view quote, std::string_view api_key, const int queue_fd) noexcept :
  _base_currency(base),
  _quote_currency(quote),
  _api_key(api_key),
  _price_exponent(0),
  _qty_exponent(0),
  _ssl_ctx(ssl::context::tlsv13_client),
  _resolver(_io_ctx),
  _ws_stream(_io_ctx, _ssl_ctx),
  _read_buffer(8192),
  _queue(queue_fd),
  _last_pair_info{}
{
  _last_pair_info.type = messages::InternalMessage::Type::PairInfo;
  std::strncpy(_last_pair_info.pair_info.base_currency, _base_currency.data(), 8);
  std::strncpy(_last_pair_info.pair_info.quote_currency, _quote_currency.data(), 8);

  _ssl_ctx.set_default_verify_paths();
  _ssl_ctx.set_verify_mode(ssl::verify_peer);
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

  _ws_stream.async_handshake(HOSTNAME, "/ws/" + _base_currency + _quote_currency + "@bestBidAsk",
    [this](const beast::error_code &ec) {
      onWSHandshake(ec);
    });
}

COLD void BinanceClient::onWSHandshake(const beast::error_code &ec)
{
  assert(!ec);

  _ws_stream.control_callback(
  [this](websocket::frame_type kind, std::string_view payload) {
    onControl(kind, payload);
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

HOT void BinanceClient::onControl(websocket::frame_type kind, std::string_view payload)
{
  switch (kind)
  {
    case websocket::frame_type::ping:
      onPing(payload);
      break;
    case websocket::frame_type::close:
      //handle close
      break;
    default:
      break;
  }
}

HOT void BinanceClient::onPing(std::string_view payload)
{
  websocket::ping_data pong_payload(payload); //useless copy, but required by the API

  _ws_stream.async_pong(pong_payload,
    [this](const beast::error_code &ec) {
      onPong(ec);
    });
}

HOT void BinanceClient::onPong(const beast::error_code &ec)
{
  assert(!ec);
}

//TODO REMOVE
#include <iostream>
HOT void BinanceClient::processData(std::span<const std::byte> data)
{
  using namespace messages::binance::sbe;
  using namespace messages;

  const Header *header = reinterpret_cast<const Header *>(data.data());
  assert(header->template_id == 10001);

  data = data.subspan(sizeof(Header));

  const BestBidAskStreamEvent *event = reinterpret_cast<const BestBidAskStreamEvent *>(data.data());

  bool info_changed = false;
  info_changed |= (_last_pair_info.pair_info.price_exponent != event->price_exponent);
  info_changed |= (_last_pair_info.pair_info.qty_exponent != event->qty_exponent);

  if (info_changed) [[unlikely]]
  {
    _last_pair_info.pair_info.price_exponent = event->price_exponent;
    _last_pair_info.pair_info.qty_exponent = event->qty_exponent;

    _queue.push(_last_pair_info);
  }

  InternalMessage message;
  message.type = InternalMessage::Type::TopOfBook;
  message.top_of_book.bid_price = event->bid_price;
  message.top_of_book.bid_qty = event->bid_qty;
  message.top_of_book.ask_price = event->ask_price;
  message.top_of_book.ask_qty = event->ask_qty;

  _queue.push(message);
}
