/*================================================================================

File: MarketDataClient.hpp                                                         
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-10 20:46:08                                                

================================================================================*/

#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>

#include "SharedSnapshot.hpp"
#include "messages/InternalMessages.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

using currency_pair = std::pair<std::string, std::string>;
using namespace messages::internal;

class MarketDataClient
{
  public:
    MarketDataClient(const currency_pair &pair, SharedSnapshot<TopOfBook> &book_snapshot, SharedSnapshot<PairInfo> &info_snapshot, std::string_view api_key) noexcept;
    ~MarketDataClient();

    void start(void) noexcept;

  private:
    MarketDataClient(const MarketDataClient &) = delete;
    MarketDataClient(MarketDataClient &&) = delete;
    MarketDataClient &operator=(const MarketDataClient &) = delete;

    static constexpr char HOSTNAME[] = "stream-sbe.binance.com";
    static constexpr char PORT[] = "9443";

    void onResolve(const beast::error_code &ec, const tcp::resolver::results_type &results);
    void onConnect(const beast::error_code &ec, const tcp::resolver::results_type::endpoint_type &endpoint);
    void onSSLHandshake(const beast::error_code &ec);
    void onWSHandshake(const beast::error_code &ec);
    void onFirstRead(const beast::error_code &ec);
    void onRead(const beast::error_code &ec);
    void onControl(websocket::frame_type kind, std::string &&payload);
    void onPing(std::string &&payload);
    void onPong(const beast::error_code &ec);
    void onClose(void);

    void asyncRead(void);

    static std::span<const std::byte> getSpan(const beast::flat_buffer &buffer);

    const currency_pair _pair;
    int8_t _price_exponent;
    int8_t _qty_exponent;
    const std::string _api_key;
    net::io_context _io_ctx;
    ssl::context _ssl_ctx;
    tcp::resolver _resolver;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> _ws_stream;
    beast::flat_buffer _read_buffer;
    SharedSnapshot<TopOfBook> &_book_snapshot;
    SharedSnapshot<PairInfo> &_info_snapshot;
};