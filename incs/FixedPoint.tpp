/*================================================================================

File: FixedPoint.tpp                                                            
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-09 16:10:34                                                 
last edited: 2025-06-12 19:33:34                                                

================================================================================*/

#pragma once

#include "FixedPoint.hpp"

#include <cmath>

template <uint8_t IntegerBits, uint8_t FractionalBits>
constexpr FixedPoint<IntegerBits, FractionalBits>::FixedPoint(void) noexcept
  : _value(0) {}

template <uint8_t IntegerBits, uint8_t FractionalBits>
constexpr FixedPoint<IntegerBits, FractionalBits>::FixedPoint(const int32_t integer) noexcept
  : _value(integer << FractionalBits) {}

template <uint8_t IntegerBits, uint8_t FractionalBits>
constexpr FixedPoint<IntegerBits, FractionalBits>::FixedPoint(const double floating_point) noexcept
  : _value(static_cast<int32_t>(floating_point * SCALE)) {}

template <uint8_t IntegerBits, uint8_t FractionalBits>
constexpr FixedPoint<IntegerBits, FractionalBits>::FixedPoint(const FixedPoint &other) noexcept
  : _value(other._value) {}

template <uint8_t IntegerBits, uint8_t FractionalBits>
constexpr FixedPoint<IntegerBits, FractionalBits>::FixedPoint(const FixedPoint &&other) noexcept
  : _value(other._value) {}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator=(const FixedPoint &other) noexcept
{
  if (this != &other)
    _value = other._value;

  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator=(const int32_t integer) noexcept
{
  _value = integer << FractionalBits;
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator=(const double floating_point) noexcept
{
  _value = static_cast<int32_t>(floating_point * SCALE);
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits>::operator int(void) const noexcept
{
  return static_cast<int>(_value >> FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits>::operator double(void) const noexcept
{
  return static_cast<double>(_value) / SCALE;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator+(const FixedPoint &other) const noexcept
{
  return FixedPoint(_value + other._value);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator+(const int32_t value) const noexcept
{
  return FixedPoint(_value + (value << FractionalBits));
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator-(const FixedPoint &other) const noexcept
{
  return FixedPoint(_value - other._value);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator-(const int32_t value) const noexcept
{
  return FixedPoint(_value - (value << FractionalBits));
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator*(const FixedPoint &other) const noexcept
{
  const int64_t product = static_cast<int64_t>(_value) * other._value;
  return FixedPoint(static_cast<int32_t>(product >> FractionalBits));
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator*(const int32_t value) const noexcept
{
  return FixedPoint((_value * value) >> FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator/(const FixedPoint &other) const noexcept
{
  const int64_t numerator = static_cast<int64_t>(_value) << FractionalBits;
  const int64_t result = numerator / other._value;
  return FixedPoint(static_cast<int32_t>(result));
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator/(const int32_t value) const noexcept
{
  return FixedPoint((_value << FractionalBits) / value);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator+=(const FixedPoint &other) noexcept
{
  _value += other._value;
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator+=(const int32_t value) noexcept
{
  _value += (value << FractionalBits);
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator-=(const FixedPoint &other) noexcept
{
  _value -= other._value;
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator-=(const int32_t value) noexcept
{
  _value -= (value << FractionalBits);
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator*=(const FixedPoint &other) noexcept
{
  const int64_t product = static_cast<int64_t>(_value) * other._value;
  _value = static_cast<int32_t>(product >> FractionalBits);
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator*=(const int32_t value) noexcept
{
  _value = (_value * value) >> FractionalBits;
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator/=(const FixedPoint &other) noexcept
{
  const int64_t numerator = static_cast<int64_t>(_value) << FractionalBits;
  _value = static_cast<int32_t>(numerator / other._value);
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> &FixedPoint<IntegerBits, FractionalBits>::operator/=(const int32_t value) noexcept
{
  _value = (_value << FractionalBits) / value;
  return *this;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator==(const FixedPoint &other) const noexcept
{
  return _value == other._value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator==(const int32_t value) const noexcept
{
  return _value == (value << FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator!=(const FixedPoint &other) const noexcept
{
  return _value != other._value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator!=(const int32_t value) const noexcept
{
  return _value != (value << FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator<(const FixedPoint &other) const noexcept
{
  return _value < other._value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator<(const int32_t value) const noexcept
{
  return _value < (value << FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator<=(const FixedPoint &other) const noexcept
{
  return _value <= other._value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator<=(const int32_t value) const noexcept
{
  return _value <= (value << FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator>(const FixedPoint &other) const noexcept
{
  return _value > other._value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator>(const int32_t value) const noexcept
{
  return _value > (value << FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator>=(const FixedPoint &other) const noexcept
{
  return _value >= other._value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr bool FixedPoint<IntegerBits, FractionalBits>::operator>=(const int32_t value) const noexcept
{
  return _value >= (value << FractionalBits);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr int32_t FixedPoint<IntegerBits, FractionalBits>::raw_value(void) const noexcept
{
  return _value;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr int32_t FixedPoint<IntegerBits, FractionalBits>::integer_part(void) const noexcept
{
  return _value >> FractionalBits;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr int32_t FixedPoint<IntegerBits, FractionalBits>::fractional_part(void) const noexcept
{
  return _value & FRACTION_MASK;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::fromRaw(const int32_t raw_value) noexcept
{
  FixedPoint result;
  result._value = raw_value;
  return result;
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
constexpr FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::log2(const uint64_t value) noexcept
{
  const uint32_t integer_part = 63 - __builtin_clzll(value);

  constexpr uint32_t TABLE_BITS = std::min(10U, FractionalBits); //2^10 = 1024 entries
  constexpr uint32_t TABLE_SIZE = 1U << TABLE_BITS;
  constexpr int32_t  SCALE = 1U << FractionalBits;
  constexpr uint32_t SHIFT_AMOUNT = 63 - TABLE_BITS;
  constexpr uint32_t MASK = TABLE_SIZE - 1;

  static constexpr std::array<uint32_t, TABLE_SIZE> log2_table = []()
  {
    std::array<uint32_t, TABLE_SIZE> table{};

    for (uint32_t i = 0; i < TABLE_SIZE; ++i)
    {
      const double x = static_cast<double>(i) / TABLE_SIZE;
      const double log2_val = std::log2(1.0 + x);
      table[i] = (log2_val * SCALE + 0.5);
    }

    return table;
  }();

  const uint32_t mantissa_shift = 63 - integer_part;
  const uint64_t shifted = value << mantissa_shift;
  const uint32_t table_index = (shifted >> SHIFT_AMOUNT) & MASK;

  const uint32_t fractional_part = log2_table[table_index];
  const int32_t raw_value = (static_cast<int32_t>(integer_part) << FractionalBits) + static_cast<int32_t>(fractional_part);

  return FixedPoint<IntegerBits, FractionalBits>::fromRaw(raw_value);
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr std::array<FixedPoint<IntegerBits, FractionalBits>, 4> FixedPoint<IntegerBits, FractionalBits>::log2(const std::array<uint64_t, 4> &values) noexcept
{
  //TODO SIMD
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline constexpr std::array<FixedPoint<IntegerBits, FractionalBits>, 8> FixedPoint<IntegerBits, FractionalBits>::log2(const std::array<uint64_t, 8> &values) noexcept
{
  //TODO SIMD
}

template <uint8_t IntegerBits, uint8_t FractionalBits>
inline std::ostream &operator<<(std::ostream &os, const FixedPoint<IntegerBits, FractionalBits> &fp) noexcept
{
  os << static_cast<double>(fp);
  return os;
}