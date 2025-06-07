#pragma once

#include "TopOfBook.hpp"

#include <cstdint>
#include <string_view>
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/endian/arithmetic.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;
using namespace boost::endian;

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

    void onResolve(const beast::error_code &ec, const tcp::resolver::results_type &results);
    void onConnect(const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint);
    void onSSLHandshake(const beast::error_code &ec);
    void onWSHandshake(const beast::error_code &ec);
    void onSubscribe(const beast::error_code &ec, const size_t bytes_transferred);
    void onRead(const beast::error_code &ec, const size_t bytes_transferred);
    void onControl(websocket::frame_type kind, std::string_view payload);
    void onPing(const std::string_view payload);
    void onPong(const beast::error_code &ec);

    inline void asyncRead(void);
    size_t processData(const std::span<const std::byte> data);

    struct BestBidAskStreamEvent
    {
      big_uint64_t event_time;
      big_uint64_t book_update_id;
      uint8_t price_exponent;
      uint8_t qty_exponent;
      big_uint64_t bid_price;
      big_uint64_t bid_qty;
      big_uint64_t ask_price;
      big_uint64_t ask_qty;
      uint8_t symbol_len;
      //variable length symbol string
    };

    const std::string _symbol;
    const std::string _api_key;
    uint8_t _price_exponent;
    uint8_t _qty_exponent;
    TopOfBook<uint64_t, uint64_t> _top_of_book;
    net::io_context _io_ctx;
    ssl::context _ssl_ctx;
    tcp::resolver _resolver;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> _ws_stream;
    beast::flat_buffer _read_buffer;
};

#include "BinanceClient.inl"