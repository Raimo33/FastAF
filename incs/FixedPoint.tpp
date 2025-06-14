/*================================================================================

File: FixedPoint.tpp                                                            
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-09 16:10:34                                                 
last edited: 2025-06-14 19:36:05                                                

================================================================================*/

#pragma once

#include "FixedPoint.hpp"
#include "macros.hpp"

#include <cmath>

template <uint8_t IntBits, uint8_t FracBits>
constexpr FixedPoint<IntBits, FracBits>::FixedPoint(void) noexcept
  : _value(0) {}

template <uint8_t IntBits, uint8_t FracBits>
constexpr FixedPoint<IntBits, FracBits>::FixedPoint(const int32_t integer) noexcept
  : _value(integer << FracBits) {}

template <uint8_t IntBits, uint8_t FracBits>
constexpr FixedPoint<IntBits, FracBits>::FixedPoint(const double floating_point) noexcept
  : _value(static_cast<int32_t>(floating_point * SCALE)) {}

template <uint8_t IntBits, uint8_t FracBits>
constexpr FixedPoint<IntBits, FracBits>::FixedPoint(const FixedPoint &other) noexcept
  : _value(other._value) {}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator=(const FixedPoint &other) noexcept
{
  _value = other._value;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator=(const int32_t integer) noexcept
{
  _value = integer << FracBits;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator=(const double floating_point) noexcept
{
  _value = static_cast<int32_t>(floating_point * SCALE);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits>::operator int(void) const noexcept
{
  return static_cast<int>(_value >> FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits>::operator double(void) const noexcept
{
  return static_cast<double>(_value) / SCALE;
}

template <uint8_t IntBits, uint8_t FracBits>
HOT inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator+(const FixedPoint &other) const noexcept
{
  return FixedPoint::fromRaw(_value + other._value);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator+(const int32_t value) const noexcept
{
  return FixedPoint::fromRaw(_value + (value << FracBits));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator+(const double value) const noexcept
{
  return FixedPoint::fromRaw(_value + static_cast<int32_t>(value * SCALE));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator-(const FixedPoint &other) const noexcept
{
  return FixedPoint::fromRaw(_value - other._value);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator-(const int32_t value) const noexcept
{
  return FixedPoint::fromRaw(_value - (value << FracBits));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator-(const double value) const noexcept
{
  return FixedPoint::fromRaw(_value - static_cast<int32_t>(value * SCALE));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator*(const FixedPoint &other) const noexcept
{
  const int64_t product = static_cast<int64_t>(_value) * other._value;
  return FixedPoint::fromRaw(static_cast<int32_t>(product >> FracBits));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator*(const int32_t value) const noexcept
{
  return FixedPoint::fromRaw(_value * value);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator*(const double value) const noexcept
{
  return FixedPoint::fromRaw((_value * static_cast<int32_t>(value * SCALE)) >> FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator/(const FixedPoint &other) const noexcept
{
  const int64_t numerator = static_cast<int64_t>(_value) << FracBits;
  const int64_t result = numerator / other._value;
  return FixedPoint::fromRaw(static_cast<int32_t>(result));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator/(const int32_t value) const noexcept
{
  return FixedPoint::fromRaw(_value / value);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator/(const double value) const noexcept
{
  return FixedPoint::fromRaw((_value << FracBits) / static_cast<int32_t>(value * SCALE));
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator-(void) const noexcept
{
  return FixedPoint::fromRaw(-_value);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::operator+(void) const noexcept
{
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator+=(const FixedPoint &other) noexcept
{
  _value += other._value;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator+=(const int32_t value) noexcept
{
  _value += (value << FracBits);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator+=(const double value) noexcept
{
  _value += static_cast<int32_t>(value * SCALE);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator-=(const FixedPoint &other) noexcept
{
  _value -= other._value;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator-=(const int32_t value) noexcept
{
  _value -= (value << FracBits);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator-=(const double value) noexcept
{
  _value -= static_cast<int32_t>(value * SCALE);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator*=(const FixedPoint &other) noexcept
{
  const int64_t product = static_cast<int64_t>(_value) * other._value;
  _value = static_cast<int32_t>(product >> FracBits);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator*=(const int32_t value) noexcept
{
  _value *= value;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator*=(const double value) noexcept
{
  _value = (_value * static_cast<int32_t>(value * SCALE)) >> FracBits;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator/=(const FixedPoint &other) noexcept
{
  const int64_t numerator = static_cast<int64_t>(_value) << FracBits;
  _value = static_cast<int32_t>(numerator / other._value);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator/=(const int32_t value) noexcept
{
  _value /= value;
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr FixedPoint<IntBits, FracBits> &FixedPoint<IntBits, FracBits>::operator/=(const double value) noexcept
{
  _value = (_value << FracBits) / static_cast<int32_t>(value * SCALE);
  return *this;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator==(const FixedPoint &other) const noexcept
{
  return _value == other._value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator==(const int32_t value) const noexcept
{
  return _value == (value << FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator==(const double value) const noexcept
{
  return _value == static_cast<int32_t>(value * SCALE);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator!=(const FixedPoint &other) const noexcept
{
  return _value != other._value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator!=(const int32_t value) const noexcept
{
  return _value != (value << FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator!=(const double value) const noexcept
{
  return _value != static_cast<int32_t>(value * SCALE);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator<(const FixedPoint &other) const noexcept
{
  return _value < other._value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator<(const int32_t value) const noexcept
{
  return _value < (value << FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
HOT inline constexpr bool FixedPoint<IntBits, FracBits>::operator<(const double value) const noexcept
{
  return _value < static_cast<int32_t>(value * SCALE);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator<=(const FixedPoint &other) const noexcept
{
  return _value <= other._value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator<=(const int32_t value) const noexcept
{
  return _value <= (value << FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator<=(const double value) const noexcept
{
  return _value <= static_cast<int32_t>(value * SCALE);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator>(const FixedPoint &other) const noexcept
{
  return _value > other._value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator>(const int32_t value) const noexcept
{
  return _value > (value << FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
HOT inline constexpr bool FixedPoint<IntBits, FracBits>::operator>(const double value) const noexcept
{
  return _value > static_cast<int32_t>(value * SCALE);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator>=(const FixedPoint &other) const noexcept
{
  return _value >= other._value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator>=(const int32_t value) const noexcept
{
  return _value >= (value << FracBits);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr bool FixedPoint<IntBits, FracBits>::operator>=(const double value) const noexcept
{
  return _value >= static_cast<int32_t>(value * SCALE);
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr int32_t FixedPoint<IntBits, FracBits>::raw_value(void) const noexcept
{
  return _value;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr int32_t FixedPoint<IntBits, FracBits>::integer_part(void) const noexcept
{
  return _value >> FracBits;
}

template <uint8_t IntBits, uint8_t FracBits>
inline constexpr int32_t FixedPoint<IntBits, FracBits>::fractional_part(void) const noexcept
{
  return _value & FRACTION_MASK;
}

template <uint8_t IntBits, uint8_t FracBits>
HOT ALWAYS_INLINE inline constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::fromRaw(const int32_t raw_value) noexcept
{
  FixedPoint result;
  result._value = raw_value;
  return result;
}

template <uint8_t IntBits, uint8_t FracBits>
HOT constexpr FixedPoint<IntBits, FracBits> FixedPoint<IntBits, FracBits>::log2(const uint64_t value) noexcept
{
  static constexpr uint8_t  TABLE_BITS = std::min(8u, static_cast<uint32_t>(FracBits));
  static constexpr uint16_t TABLE_SIZE = 1 << TABLE_BITS;
  static constexpr uint8_t  SHIFT_AMOUNT = 63 - TABLE_BITS;
  static constexpr uint16_t MASK = TABLE_SIZE - 1;
  static constexpr uint8_t  FRAC_BITS_FOR_INTERP = FracBits - TABLE_BITS;
  static constexpr uint8_t  FRAC_SHIFT_OFFSET = SHIFT_AMOUNT - FRAC_BITS_FOR_INTERP;
  static constexpr uint16_t FRAC_MASK = (1u << FRAC_BITS_FOR_INTERP) - 1;

  alignas(CACHELINE_SIZE) static const constinit std::array<uint32_t, TABLE_SIZE + 1> log2_table = []()
  {
    std::array<uint32_t, TABLE_SIZE + 1> table{};

    for (uint32_t i = 0; i <= TABLE_SIZE; ++i)
    {
      const double x = static_cast<double>(i) / TABLE_SIZE;
      const double log2_val = std::log2(1.0 + x);
      table[i] = static_cast<uint32_t>(log2_val * SCALE + 0.5);
    }
    return table;
  }();

  const uint32_t leading_zeroes = __builtin_clzll(value);
  const uint64_t shifted = value << leading_zeroes;
  const uint32_t table_index = (shifted >> SHIFT_AMOUNT) & MASK;

  PREFETCH_R(&log2_table[table_index], 0);

  const uint32_t integer_part = 63 - leading_zeroes;
  const uint32_t frac_offset = (shifted >> FRAC_SHIFT_OFFSET) & FRAC_MASK;
  const uint32_t val0 = log2_table[table_index];
  const uint32_t val1 = log2_table[table_index + 1];
  const uint64_t diff = static_cast<uint64_t>(val1) - val0;
  const uint64_t prod = diff * frac_offset;
  const uint32_t interp = val0 + static_cast<uint32_t>(prod >> FRAC_BITS_FOR_INTERP);

  const int32_t raw_value = (static_cast<int32_t>(integer_part) << FracBits) + static_cast<int32_t>(interp);
  return FixedPoint<IntBits, FracBits>::fromRaw(raw_value);
}


template <uint8_t IntBits, uint8_t FracBits>
inline std::ostream &operator<<(std::ostream &os, const FixedPoint<IntBits, FracBits> &fp) noexcept
{
  os << static_cast<double>(fp);
  return os;
}