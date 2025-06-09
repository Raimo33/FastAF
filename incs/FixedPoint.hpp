/*================================================================================

File: FixedPoint.hpp                                                            
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-09 16:10:33                                                 
last edited: 2025-06-09 16:10:33                                                

================================================================================*/

#pragma once

#include <cstdint>
#include <array>

template <uint8_t IntegerBits, uint8_t FractionalBits = (32 - IntegerBits)>
  requires(IntegerBits > 0 && IntegerBits + FractionalBits == 32)
class FixedPoint
{
  public:
    constexpr FixedPoint(void) noexcept;
    explicit constexpr FixedPoint(const int32_t integer) noexcept;
    explicit constexpr FixedPoint(const double floating_point) noexcept;

    constexpr FixedPoint(const FixedPoint &other) noexcept;
    constexpr FixedPoint(const FixedPoint &&other) noexcept;
    inline constexpr FixedPoint &operator=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator=(const int32_t integer) noexcept;
    inline constexpr FixedPoint &operator=(const double floating_point) noexcept;

    explicit constexpr operator int(void) const noexcept;
    explicit constexpr operator double(void) const noexcept;

    inline constexpr FixedPoint operator+(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator+(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator-(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator-(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator*(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator*(const int32_t value) const noexcept;
    inline constexpr FixedPoint operator/(const FixedPoint &other) const noexcept;
    inline constexpr FixedPoint operator/(const int32_t value) const noexcept;
    inline constexpr FixedPoint &operator+=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator+=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator-=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator-=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator*=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator*=(const int32_t value) noexcept;
    inline constexpr FixedPoint &operator/=(const FixedPoint &other) noexcept;
    inline constexpr FixedPoint &operator/=(const int32_t value) noexcept;
    inline constexpr bool operator==(const FixedPoint &other) const noexcept;
    inline constexpr bool operator==(const int32_t value) const noexcept;
    inline constexpr bool operator!=(const FixedPoint &other) const noexcept;
    inline constexpr bool operator!=(const int32_t value) const noexcept;
    inline constexpr bool operator<(const FixedPoint &other) const noexcept;
    inline constexpr bool operator<(const int32_t value) const noexcept;
    inline constexpr bool operator<=(const FixedPoint &other) const noexcept;
    inline constexpr bool operator<=(const int32_t value) const noexcept;
    inline constexpr bool operator>(const FixedPoint &other) const noexcept;
    inline constexpr bool operator>(const int32_t value) const noexcept;
    inline constexpr bool operator>=(const FixedPoint &other) const noexcept;
    inline constexpr bool operator>=(const int32_t value) const noexcept;

    constexpr int32_t raw_value(void) const noexcept;
    constexpr int32_t integer_part(void) const noexcept;
    constexpr int32_t fractional_part(void) const noexcept;

    static constexpr FixedPoint fromRaw(const int32_t raw_value) noexcept;

    static constexpr FixedPoint log2(const uint64_t value) noexcept;
    static constexpr std::array<FixedPoint, 4> log2(const std::array<uint64_t, 4> &values) noexcept;
    static constexpr std::array<FixedPoint, 8> log2(const std::array<uint64_t, 8> &values) noexcept;

  private:
    int32_t _value;
    static constexpr int32_t SCALE = 1u << FractionalBits;
    static constexpr int32_t FRACTION_MASK = SCALE - 1;
    static constexpr int32_t MAX_INTEGER_VALUE = 1u << IntegerBits;
};

#include "FixedPoint.tpp"
