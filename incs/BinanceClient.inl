#pragma once

#include "BinanceClient.hpp"
#include "macros.hpp"

inline void BinanceClient::asyncRead(void)
{
  _ws_stream.async_read(_read_buffer,
    [this](const beast::error_code &ec, UNUSED std::size_t bytes_transferred) {
      onRead(ec);
    });
}