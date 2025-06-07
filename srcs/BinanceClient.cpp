#include "BinanceClient.hpp"

BinanceClient::BinanceClient(std::string_view symbol, std::string_view api_key) noexcept :
  _symbol(symbol),
  _api_key(api_key),
  _price_exponent(0),
  _qty_exponent(0),
  _ssl_ctx(ssl::context::tlsv13_client),
  _resolver(_io_ctx),
  _ws_stream(_io_ctx, _ssl_ctx),
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
  if (ec) [[unlikely]]
    utils::throw_error("Failed to resolve Binance WebSocket endpoint: " + ec.message());

  //log

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
  if (ec) [[unlikely]]
    utils::throw_error("Failed to connect to Binance WebSocket endpoint: " + ec.message());

  //log

  auto &stream = _ws_stream.next_layer();

  stream.async_handshake(ssl::stream_base::client,
    [this](const beast::error_code &ec) {
      onSSLHandshake(ec);
    });
}

void BinanceClient::onSSLHandshake(const beast::error_code &ec)
{
  if (ec) [[unlikely]]
    utils::throw_error("SSL handshake failed: " + ec.message());

  //log

  _ws_stream.async_handshake(HOSTNAME, "/ws/" + _symbol + "@bestBidAsk",
    [this](const beast::error_code &ec) {
      onWSHandshake(ec);
    });
}

void BinanceClient::onWSHandshake(const beast::error_code &ec)
{
  if (ec) [[unlikely]]
    utils::throw_error("WebSocket handshake failed: " + ec.message());

  //log

  _ws_stream.control_callback(
    [this](websocket::frame_type kind, std::string_view payload) {
      onControl(kind, payload);
    });

  asyncRead();
}

void BinanceClient::onRead(const beast::error_code &ec, const size_t bytes_transferred)
{
  //check error branchless (every once in a while with a timer)

  if (_ws_stream.got)

  asyncRead();
}

void BinanceClient::onControl(websocket::frame_type kind, std::string_view payload)
{
  switch (kind) {
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
  _ws_stream.async_pong(payload,
    [this](const beast::error_code &ec) {
      onPong(ec);
    });
}

void BinanceClient::onPong(const beast::error_code &ec)
{
  if (ec) [[unlikely]]
    utils::throw_error("Failed to send pong: " + ec.message());

  //log
}
