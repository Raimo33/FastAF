/*================================================================================

File: InternalMessages.hpp                                                      
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#pragma once

#include <cstdint>

#pragma pack(push, 1)

namespace messages
{

union InternalMessage
{
  enum Type : uint8_t
  {
    TopOfBook = 0x01,
    PairInfo  = 0x02
  } type;

  struct TopOfBook
  {
    uint64_t bid_price;
    uint64_t bid_qty;
    uint64_t ask_price;
    uint64_t ask_qty;
  } top_of_book;

  struct PairInfo
  {
    uint8_t price_exponent;
    uint8_t qty_exponent;
    char base_currency[8];
    char quote_currency[8];
  } pair_info;
};

} // namespace messages

#pragma pack(pop)