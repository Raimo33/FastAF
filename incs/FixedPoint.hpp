/*================================================================================

File: FixedPoint.hpp                                                            
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-09 16:10:33                                                 
last edited: 2025-06-13 21:01:01                                                

================================================================================*/

#pragma once

#include <cstdint>
#include <array>
#include <ostream>

template <uint8_t IntBits, uint8_t FracBits = (32 - IntBits)>
class FixedPoint
{
  static_assert(IntBits + FracBits <= 32, "Total bits must not exceed 32");

  public:
    constexpr FixedPoint(void) noexcept;
    explicit constexpr FixedPoint(const int32_t integer) noexcept;
    explicit constexpr FixedPoint(const double floating_point) noexcept;

    constexpr FixedPoint(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator=(const int32_t integer) noexcept;
    inline constexpr FixedPoint &operator=(const double floating_point) noexcept;

    explicit constexpr operator int(void) const noexcept;
    explicit constexpr operator double(void) const noexcept;

    inline constexpr FixedPoint operator+(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator+(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator+(const double value) const noexcept;
    inline constexpr FixedPoint operator-(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator-(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator-(const double value) const noexcept;
    inline constexpr FixedPoint operator*(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator*(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator*(const double value) const noexcept;
    inline constexpr FixedPoint operator/(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator/(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator/(const double value) const noexcept;

    inline constexpr FixedPoint operator-(void) const noexcept;
    inline constexpr FixedPoint operator+(void) const noexcept;

    inline constexpr FixedPoint &operator+=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator+=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator+=(const double value) noexcept;
    inline constexpr FixedPoint &operator-=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator-=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator-=(const double value) noexcept;
    inline constexpr FixedPoint &operator*=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator*=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator*=(const double value) noexcept;
    inline constexpr FixedPoint &operator/=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator/=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator/=(const double value) noexcept;

    inline constexpr bool operator==(const FixedPoint &other) const noexcept;
    inline constexpr bool operator==(const int32_t value) const noexcept;
    inline constexpr bool operator==(const double value) const noexcept;
    inline constexpr bool operator!=(const FixedPoint &other) const noexcept;
    inline constexpr bool operator!=(const int32_t value) const noexcept;
    inline constexpr bool operator!=(const double value) const noexcept;
    inline constexpr bool operator<(const FixedPoint &other) const noexcept;
    inline constexpr bool operator<(const int32_t value) const noexcept;
    inline constexpr bool operator<(const double value) const noexcept;
    inline constexpr bool operator<=(const FixedPoint &other) const noexcept;
    inline constexpr bool operator<=(const int32_t value) const noexcept;
    inline constexpr bool operator<=(const double value) const noexcept;
    inline constexpr bool operator>(const FixedPoint &other) const noexcept;
    inline constexpr bool operator>(const int32_t value) const noexcept;
    inline constexpr bool operator>(const double value) const noexcept;
    inline constexpr bool operator>=(const FixedPoint &other) const noexcept;
    inline constexpr bool operator>=(const int32_t value) const noexcept;
    inline constexpr bool operator>=(const double value) const noexcept;

    constexpr int32_t raw_value(void) const noexcept;
    constexpr int32_t integer_part(void) const noexcept;
    constexpr int32_t fractional_part(void) const noexcept;

    static constexpr FixedPoint fromRaw(const int32_t raw_value) noexcept;

    static constexpr FixedPoint log2(const uint64_t value) noexcept;

    //add4
    //add8
    //add16

  private:
    int32_t _value;
    static constexpr int32_t SCALE = 1u << FracBits;
    static constexpr int32_t FRACTION_MASK = SCALE - 1;
    static constexpr int32_t MAX_INTEGER_VALUE = 1u << IntBits;
};

template <uint8_t IntBits, uint8_t FracBits>
inline std::ostream &operator<<(std::ostream &os, const FixedPoint<IntBits, FracBits> &value) noexcept;

#include "FixedPoint.tpp"
