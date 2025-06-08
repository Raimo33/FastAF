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

namespace internal
{
  struct TopOfBook
  {
    uint64_t bid_price;
    uint64_t bid_qty;
    uint64_t ask_price;
    uint64_t ask_qty;
    //could be pushed once every change
    uint8_t price_exponent;
    uint8_t qty_exponent;
  };

} // namespace internal

} // namespace messages

#pragma pack(pop)