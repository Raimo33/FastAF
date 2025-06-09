/*================================================================================

File: InternalMessages.hpp                                                      
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-09 20:07:26                                                

================================================================================*/

#pragma once

#include <cstdint>

#pragma pack(push, 1)

namespace messages
{

namespace internal
{

struct TopOfBook
{
  int64_t bid_price;
  int64_t bid_qty;
  int64_t ask_price;
  int64_t ask_qty;
};

struct PairInfo
{
  int8_t price_exponent;
  int8_t qty_exponent;
};

} // namespace internal

} // namespace messages

#pragma pack(pop)