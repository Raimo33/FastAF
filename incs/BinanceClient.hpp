#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>

#include "ipq/SPSCQueue.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

class BinanceClient
{
  public:
    using queue_capacity = 64;

    BinanceClient(std::string_view symbol, std::string_view api_key, const int queue_fd) noexcept;
    ~BinanceClient() = default;

    void start(void) noexcept;

    static constexpr char HOSTNAME[] = "stream-sbe.binance.com";
    static constexpr char PORT[] = "9443";

  private:
    BinanceClient(const BinanceClient &) = delete;
    BinanceClient &operator=(const BinanceClient &) = delete;

    void onResolve(const beast::error_code &ec, const tcp::resolver::results_type &results);
    void onConnect(const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint);
    void onSSLHandshake(const beast::error_code &ec);
    void onWSHandshake(const beast::error_code &ec);
    void onRead(const beast::error_code &ec);
    void onControl(websocket::frame_type kind, std::string_view payload);
    void onPing(const std::string_view payload);
    void onPong(const beast::error_code &ec);

    void asyncRead(void);
    void processData(const std::span<const std::byte> data);

    const std::string _symbol;
    const std::string _api_key;
    uint8_t _price_exponent;
    uint8_t _qty_exponent;
    net::io_context _io_ctx;
    ssl::context _ssl_ctx;
    tcp::resolver _resolver;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> _ws_stream;
    beast::flat_buffer _read_buffer;
    ipq::SPSCQueue<messages::internal::InternalMessage, queue_capacity> _queue;
};