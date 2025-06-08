#pragma once

#include <boost/endian/arithmetic.hpp>

using namespace boost::endian;

#pragma pack(push, 1)

namespace messages
{

namespace binance
{

namespace sbe
{

struct Header
{
  little_uint16_t block_length;
  little_uint16_t template_id;
  little_uint16_t schema_id;
  little_uint16_t version;
};

struct BestBidAskStreamEvent
{
  little_uint64_t event_time;
  little_uint64_t book_update_id;
  uint8_t price_exponent;
  uint8_t qty_exponent;
  little_uint64_t bid_price;
  little_uint64_t bid_qty;
  little_uint64_t ask_price;
  little_uint64_t ask_qty;
  uint8_t symbol_len;
  //variable length symbol string
};

} // namespace sbe

} // namespace binance

} // namespace messages

#pragma pack(pop)