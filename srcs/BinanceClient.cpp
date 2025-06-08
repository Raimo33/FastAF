#include "BinanceClient.hpp"
#include "Messages.hpp"

using namespace binance::sbe;

BinanceClient::BinanceClient(std::string_view symbol, std::string_view api_key) noexcept :
  _symbol(symbol),
  _api_key(api_key),
  _price_exponent(0),
  _qty_exponent(0),
  _ssl_ctx(ssl::context::tlsv13_client),
  _resolver(_io_ctx),
  _ws_stream(_io_ctx, _ssl_ctx),
  _read_buffer(8192)
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

  auto &stream = beast::get_lowest_layer(_ws_stream);

  stream.async_connect(results,
    [this](const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint) {
      onConnect(ec, endpoint);
    });
}

void BinanceClient::onConnect(const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint)
{
  assert(!ec);

  //log
  (void)endpoint;

  auto &stream = _ws_stream.next_layer();
  auto &socket = beast::get_lowest_layer(stream).socket();

  socket.set_option(tcp::no_delay(true));

  stream.async_handshake(ssl::stream_base::client,
    [this](const beast::error_code &ec) {
      onSSLHandshake(ec);
    });
}

void BinanceClient::onSSLHandshake(const beast::error_code &ec)
{
  assert(!ec);

  //log

  _ws_stream.set_option(websocket::stream_base::decorator(
    [this](websocket::request_type &req) {
      req.set("X-MBX-APIKEY", _api_key);
    }));

  _ws_stream.async_handshake(HOSTNAME, "/ws/" + _symbol + "@bestBidAsk",
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
    [this](const beast::error_code &ec, UNUSED const size_t bytes_transferred) {
      onSubscribe(ec);
    });
}

void BinanceClient::onSubscribe(const beast::error_code &ec)
{
  assert(!ec);

  //log

  _ws_stream.control_callback(
    [this](websocket::frame_type kind, std::string_view payload) {
      onControl(kind, payload);
    });

  _ws_stream.binary(true);

  asyncRead();
}

void BinanceClient::onRead(const beast::error_code &ec)
{
  assert(!ec);
  assert(_ws_stream.got_binary());

  auto buffers = _read_buffer.data();
  const std::byte *ptr = static_cast<const std::byte *>(boost::asio::buffer_cast<const void*>(buffers));
  const size_t total = boost::asio::buffer_size(buffers);

  size_t offset = 0;

  while (true)
  {
    if (total - offset < sizeof(Header)) [[unlikely]]
      break;

    const auto *header = reinterpret_cast<const Header *>(ptr + offset);

    size_t packet_size = sizeof(Header) + header->block_length;
    if (total - offset < packet_size) [[unlikely]]
      break;

    offset += sizeof(Header);
    const auto *payload = reinterpret_cast<const BestBidAskStreamEvent *>(ptr + offset);

    packet_size = sizeof(BestBidAskStreamEvent) + payload->symbol_len;
    if (total - offset < packet_size) [[unlikely]]
      break;

    processData({ptr + offset, packet_size});
    offset += packet_size;
  }

  _read_buffer.consume(offset);
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

  _ws_stream.async_pong(pong_payload,
    [this](const beast::error_code &ec) {
      onPong(ec);
    });
}

void BinanceClient::onPong(const beast::error_code &ec)
{
  assert(!ec);

  //log
}

size_t BinanceClient::processData(std::span<const std::byte> data)
{
  const BestBidAskStreamEvent *event = reinterpret_cast<const BestBidAskStreamEvent *>(data.data());
  _price_exponent = event->price_exponent;
  _qty_exponent = event->qty_exponent;

  _top_of_book.setBestBid(event->bid_price, event->bid_qty);
  _top_of_book.setBestAsk(event->ask_price, event->ask_qty);

  printf("Best Bid: %lu, Qty: %lu, Best Ask: %lu, Qty: %lu\n", 
        _top_of_book.getBestBidPrice(),
        _top_of_book.getBestBidQty(),
        _top_of_book.getBestAskPrice(),
        _top_of_book.getBestAskQty());

  return sizeof(BestBidAskStreamEvent) + event->symbol_len;
}
