#pragma once

#include "TopOfBook.hpp"

#include <cstdint>
#include <string_view>
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class BinanceClient
{
  public:
    BinanceClient(std::string_view symbol, std::string_view api_key) noexcept;
    ~BinanceClient() = default;

    void start(void) noexcept;

    static constexpr char HOSTNAME[] = "stream-sbe.binance.com";
    static constexpr char PORT[] = "9443";

  private:
    BinanceClient(const BinanceClient &) = delete;
    BinanceClient &operator=(const BinanceClient &) = delete;

    void onResolve(const boost::system::error_code &ec, const tcp::resolver::results_type &results);
    void onConnect(const boost::system::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint);
    void onSSLHandshake(const boost::system::error_code &ec);
    void onWSHandshake(const boost::system::error_code &ec);
    void onSubscribe(const boost::system::error_code &ec);
    void onRead(const boost::system::error_code &ec);
    void onPing(const boost::system::error_code &ec);

    const std::string _symbol;
    const std::string _api_key;
    uint8_t _price_exponent;
    uint8_t _qty_exponent;
    TopOfBook<uint64_t, uint64_t> _top_of_book;

    net::io_context _io_ctx;
    ssl::context _ssl_ctx;
    tcp::resolver _resolver;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> _ws_stream;
    beast::flat_buffer _buffer;
};