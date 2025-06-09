/*================================================================================

File: MarketDataClient.hpp                                                         
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-09 12:36:36                                                

================================================================================*/

#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>

#include "ipq/SPSCQueue.hpp"
#include "messages/InternalMessages.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

using currency_pair = std::pair<std::string, std::string>;
using namespace messages;

class MarketDataClient
{
  public:
    static constexpr size_t QUEUE_CAPACITY = 16;

    MarketDataClient(const currency_pair &pair, std::string_view api_key);
    ~MarketDataClient();

    void start(void) noexcept;

    static constexpr char HOSTNAME[] = "stream-sbe.binance.com";
    static constexpr char PORT[] = "9443";

  private:
    MarketDataClient(const MarketDataClient &) = delete;
    MarketDataClient &operator=(const MarketDataClient &) = delete;

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

    using queue_type = ipq::SPSCQueue<InternalMessage, QUEUE_CAPACITY>;
    using stream_type = websocket::stream<beast::ssl_stream<beast::tcp_stream>>;

    const currency_pair _pair;
    uint8_t _price_exponent;
    uint8_t _qty_exponent;
    const std::string _api_key;
    net::io_context _io_ctx;
    ssl::context _ssl_ctx;
    tcp::resolver _resolver;
    stream_type _ws_stream;
    beast::flat_buffer _read_buffer;
    std::string _mem_name;
    int _queue_fd;
    queue_type _queue;
};