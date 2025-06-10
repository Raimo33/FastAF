/*================================================================================

File: BinanceMessages.hpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-09 11:30:42                                                 
last edited: 2025-06-10 18:45:29                                                

================================================================================*/

#pragma once

#include <boost/endian/arithmetic.hpp>

using namespace boost::endian;

#pragma pack(push, 1)

namespace messages
{

namespace sbe
{

struct SBEMessage
{
  struct Header
  {
    little_uint16_t block_length;
    little_uint16_t template_id;
    little_uint16_t schema_id;
    little_uint16_t version;
  } header;

  struct BestBidAskStreamEvent
  {
    little_uint64_t event_time;
    little_uint64_t book_update_id;
    int8_t price_exponent;
    int8_t qty_exponent;
    little_int64_t bid_price;
    little_int64_t bid_qty;
    little_int64_t ask_price;
    little_int64_t ask_qty;
    uint8_t symbol_len;
    //variable length symbol string
  } best_bid_ask_stream_event;
};

} // namespace sbe

} // namespace messages

#pragma pack(pop)