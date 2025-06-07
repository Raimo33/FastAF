#include "BinanceClient.hpp"

BinanceClient::BinanceClient(std::string_view symbol, std::string_view api_key) noexcept :
  _symbol(symbol),
  _api_key(api_key),
  _price_exponent(0),
  _qty_exponent(0),
  _ssl_ctx(ssl::context::tlsv13_client),
  _resolver(_io_ctx),
  _ws_stream(_io_ctx, _ssl_ctx),
  _read_buffer(sizeof(BestBidAskStreamEvent) + _symbol.size())
{
  _ssl_ctx.set_default_verify_paths();
  _ssl_ctx.set_verify_mode(ssl::verify_peer);

  //set cypher list
}

void BinanceClient::start(void) noexcept
{
  _resolver.async_resolve(HOSTNAME, PORT,
    [this](const boost::system::error_code &ec, const tcp::resolver::results_type &results) {
      onResolve(ec, results);
    });

  _io_ctx.run();
}

void BinanceClient::onResolve(const beast::error_code &ec, const tcp::resolver::results_type &results)
{
  assert(!ec);

  //log
  printf("Resolved %s:%s for symbol: %s\n", HOSTNAME, PORT, _symbol.c_str());

  auto &stream = beast::get_lowest_layer(_ws_stream);
  auto &socket = stream.socket();

  socket.set_option(tcp::no_delay(true));

  stream.async_connect(results,
    [this](const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint) {
      onConnect(ec, endpoint);
    });
}

void BinanceClient::onConnect(const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint)
{
  assert(!ec);

  //log
  printf("Connected to %s:%s for symbol: %s\n", endpoint.address().to_string().c_str(), std::to_string(endpoint.port()).c_str(), _symbol.c_str());

  auto &stream = _ws_stream.next_layer();

  stream.async_handshake(ssl::stream_base::client,
    [this](const beast::error_code &ec) {
      onSSLHandshake(ec);
    });
}

void BinanceClient::onSSLHandshake(const beast::error_code &ec)
{
  assert(!ec);

  //log
  printf("SSL handshake successful for symbol: %s\n", _symbol.c_str());

  _ws_stream.async_handshake_ex(HOSTNAME, "/ws" + _symbol + "@bestBidAsk",
    [this](const beast::request_type &req) {
      req.set("X-MBX-APIKEY", _api_key);
    },
    [this](const beast::error_code &ec) {
      onWSHandshake(ec);
    });
}

void BinanceClient::onWSHandshake(const beast::error_code &ec)
{
  assert(!ec);

  //log
  printf("WebSocket handshake successful for symbol: %s\n", _symbol.c_str());

  std::string subscribe_msg = R"({
    "method": "SUBSCRIBE",
    "params": ["bestBidAsk@)" + _symbol + R"("],
    "id": 1
  })";

  _ws_stream.async_write(boost::asio::buffer(subscribe_msg),
    [this](const beast::error_code &ec, const size_t bytes_transferred) {
      onSubscribe(ec, bytes_transferred);
    });
}

void BinanceClient::onSubscribe(const beast::error_code &ec, const size_t bytes_transferred)
{
  assert(!ec);

  //log
  printf("Subscription message sent for symbol: %s, bytes transferred: %zu\n", _symbol.c_str(), bytes_transferred);

  _ws_stream.control_callback(
    [this](websocket::frame_type kind, std::string_view payload) {
      onControl(kind, payload);
    });

  _ws_stream.binary(true);

  asyncRead();
}

void BinanceClient::onRead(const beast::error_code &ec, const size_t bytes_transferred)
{
  assert(!ec);
  assert(_ws_stream.got_binary());

  auto buffers = _read_buffer.data();
  const std::byte *ptr = static_cast<const std::byte *>(boost::asio::buffer_cast<const void*>(buffers));

  const size_t bytes_processed = processData({ptr, bytes_transferred});
  _read_buffer.consume(bytes_processed);

  asyncRead();
}

void BinanceClient::onControl(websocket::frame_type kind, std::string_view payload)
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

void BinanceClient::onPing(std::string_view payload)
{
  websocket::ping_data pong_payload(payload); //useless copy, but required by the API

  //log
  printf("Received ping with payload: %.*s\n", static_cast<int>(payload.size()), payload.data());

  _ws_stream.async_pong(pong_payload,
    [this](const beast::error_code &ec) {
      onPong(ec);
    });
}

void BinanceClient::onPong(const beast::error_code &ec)
{
  assert(!ec);

  //log
  printf("Pong sent successfully for symbol: %s\n", _symbol.c_str());
}

size_t BinanceClient::processData(std::span<const std::byte> data)
{
  assert(data.size() >= sizeof(BestBidAskStreamEvent));

  const BestBidAskStreamEvent *event = reinterpret_cast<const BestBidAskStreamEvent *>(data.data());
  _price_exponent = event->price_exponent;
  _qty_exponent = event->qty_exponent;

  _top_of_book.setBestBid(event->bid_price, event->bid_qty);
  _top_of_book.setBestAsk(event->ask_price, event->ask_qty);

  return sizeof(BestBidAskStreamEvent) + event->symbol_len;
}
