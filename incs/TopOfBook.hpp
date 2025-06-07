#pragma once

#include <cstdint>
#include <type_traits>

template <typename T>
concept Integral = std::is_integral_v<T>;

template <Integral PriceType, Integral QtyType>
class TopOfBook
{
  public:
    TopOfBook(void) noexcept = default;
    ~TopOfBook() = default;

    inline PriceType getBestBidPrice(void) const noexcept;
    inline PriceType getBestAskPrice(void) const noexcept;
    inline QtyType getBestBidQty(void) const noexcept;
    inline QtyType getBestAskQty(void) const noexcept;

    inline void setBestBid(const PriceType price, const QtyType qty) noexcept;
    inline void setBestAsk(const PriceType price, const QtyType qty) noexcept;

  private:
    TopOfBook(const TopOfBook &) = delete;
    TopOfBook &operator=(const TopOfBook &) = delete;

    PriceType _best_bid_price;
    PriceType _best_ask_price;
    QtyType _best_bid_qty;
    QtyType _best_ask_qty;
};

#include "TopOfBook.inl"