/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-13 22:02:52                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "utils.hpp"
#include "macros.hpp"

#include <utility>
#include <thread>
#include <algorithm>
#include <numeric>

//TODO remove
#include <iostream>

COLD ArbitrageScanner::ArbitrageScanner(const std::array<currency_pair, 3> &pairs, std::array<SharedSnapshot<TopOfBook>, 3> &book_snapshots, std::array<SharedSnapshot<PairInfo>, 3> &info_snapshots) noexcept :
  _info_snapshots(info_snapshots),
  _book_snapshot0(book_snapshots[0]),
  _book_snapshot1(book_snapshots[1]),
  _book_snapshot2(book_snapshots[2]),
  _combined_price_exponent(0),
  _combined_qty_exponent(0)
{
  (void)pairs;
}

COLD void ArbitrageScanner::start(void)
{
  getPairInfo();
  precomputeConstants();
  initBooks();

  while (true)
  {
    _book_snapshot0.try_pop(_book0, _book_version0);
    checkArbitrage();
    _book_snapshot1.try_pop(_book1, _book_version1);
    checkArbitrage();
    _book_snapshot2.try_pop(_book2, _book_version2);
    checkArbitrage();
  }

  std::unreachable();
}

COLD void ArbitrageScanner::getPairInfo(void)
{
  std::array<size_t, 3> info_versions{0};

  for (uint8_t i = 0; i < 3; ++i)
  {
    PairInfo info{};

    while (!_info_snapshots[i].try_pop(info, info_versions[i]))
      ;

    if (i == 2)
      _combined_price_exponent -= info.price_exponent;
    else
      _combined_price_exponent += info.price_exponent;
    
    _combined_qty_exponent += info.qty_exponent;
  }
}

COLD void ArbitrageScanner::precomputeConstants(void)
{
  static constexpr double LOG_10 = std::log2(10.0);
  static constexpr double TAU = THRESHOLD_PERCENTAGE / 100.0;
  static constexpr double LOG_THRESHOLD = std::log2(1.0 + TAU);

  _neutral_point = -_combined_price_exponent * LOG_10;
  _forward_threshold  = _neutral_point + LOG_THRESHOLD;
  _backward_threshold = _neutral_point - LOG_THRESHOLD;
}

COLD void ArbitrageScanner::initBooks(void)
{
  do
  {
    _book_snapshot0.try_pop(_book0, _book_version0);
    _book_snapshot1.try_pop(_book1, _book_version1);
    _book_snapshot2.try_pop(_book2, _book_version2);
  }
  while (
    (_book_version0 == 0) |
    (_book_version1 == 0) |
    (_book_version2 == 0)
  );
}

HOT void ArbitrageScanner::checkArbitrage(void) noexcept
{
  fast_assert(
    (_book0.bid_price > 0) & (_book0.ask_price > 0) &
    (_book1.bid_price > 0) & (_book1.ask_price > 0) &
    (_book2.bid_price > 0) & (_book2.ask_price > 0)
  );

  using fixed_type = FixedPoint<8, 24>;
  static constexpr auto log2 = [](const int64_t price) -> fixed_type {
    return fixed_type::log2(static_cast<uint64_t>(price));
  };


  std::cout << "bid_price0: " << _book0.bid_price << ", ask_price0: " << _book0.ask_price << '\n';
  std::cout << "log2(bid_price0): " << log2(_book0.bid_price) << ", log2(ask_price0): " << log2(_book0.ask_price) << '\n';
  std::cout << "bid_price1: " << _book1.bid_price << ", ask_price1: " << _book1.ask_price << '\n';
  std::cout << "log2(bid_price1): " << log2(_book1.bid_price) << ", log2(ask_price1): " << log2(_book1.ask_price) << '\n';
  std::cout << "bid_price2: " << _book2.bid_price << ", ask_price2: " << _book2.ask_price << '\n';
  std::cout << "log2(bid_price2): " << log2(_book2.bid_price) << ", log2(ask_price2): " << log2(_book2.ask_price) << '\n';

  exit(1);

  // const fixed_type forward_path  = log2(_book0.bid_price) + log2(_book1.bid_price) - log2(_book2.ask_price);
  // const fixed_type backward_path = log2(_book0.ask_price) + log2(_book1.ask_price) - log2(_book2.bid_price);
//
}