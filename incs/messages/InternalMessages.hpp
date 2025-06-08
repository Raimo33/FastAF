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
};

} // namespace internal

} // namespace messages

#pragma pack(pop)