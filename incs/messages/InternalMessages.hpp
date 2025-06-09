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

union InternalMessage
{
  enum class MessageType : uint8_t
  {
    TopOfBook = 0x01,
    PairInfo  = 0x02,
  };

  struct TopOfBook
  {
    static constexpr MessageType type = MessageType::TopOfBook;
    int64_t bid_price;
    int64_t bid_qty;
    int64_t ask_price;
    int64_t ask_qty;
  } top_of_book;

  struct PairInfo
  {
    static constexpr MessageType type = MessageType::PairInfo;
    char base_currency[8];
    char quote_currency[8];
    int8_t price_exponent;
    int8_t qty_exponent;
  } pair_info;
};

} // namespace messages

#pragma pack(pop)