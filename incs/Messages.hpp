#pragma once

#include <boost/endian/arithmetic.hpp>

using namespace boost::endian;

#pragma pack(push, 1)

namespace binance
{

namespace sbe
{

struct Header
{
  big_uint16_t block_length;
  big_uint16_t template_id;
  big_uint16_t schema_id;
  big_uint16_t version;
};

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

} // namespace sbe

} // namespace binance

#pragma pack(pop)