#pragma once

#include "TopOfBook.hpp"

template <Integral PriceType, Integral QtyType>
inline PriceType TopOfBook<PriceType, QtyType>::getBestBidPrice(void) const noexcept
{
  return _best_bid_price;
}

template <Integral PriceType, Integral QtyType>
inline PriceType TopOfBook<PriceType, QtyType>::getBestAskPrice(void) const noexcept
{
  return _best_ask_price;
}

template <Integral PriceType, Integral QtyType>
inline QtyType TopOfBook<PriceType, QtyType>::getBestBidQty(void) const noexcept
{
  return _best_bid_qty;
}

template <Integral PriceType, Integral QtyType>
inline QtyType TopOfBook<PriceType, QtyType>::getBestAskQty(void) const noexcept
{
  return _best_ask_qty;
}

template <Integral PriceType, Integral QtyType>
inline void TopOfBook<PriceType, QtyType>::setBestBid(const PriceType price, const QtyType qty) noexcept
{
  _best_bid_price = price;
  _best_bid_qty = qty;
}

template <Integral PriceType, Integral QtyType>
inline void TopOfBook<PriceType, QtyType>::setBestAsk(const PriceType price, const QtyType qty) noexcept
{
  _best_ask_price = price;
  _best_ask_qty = qty;
}