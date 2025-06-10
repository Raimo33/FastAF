/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-10 18:45:29                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "macros.hpp"

#include <utility>
#include <thread>

/*

p1 * e1 + p2 * e2 + p3 * e3 > 1
p1 * p1 * p3 > 10^(e1 + e2 + e3)
log2(p1) + log2(p2) + log2(p3) > log2(10^(e1 + e2 + e3))
log2(p1) + log2(p2) + log2(p3) > (e1 + e2 + e3) * log2(10)

*/

COLD ArbitrageScanner::ArbitrageScanner(const std::array<currency_pair, 3> &pairs, std::array<SharedSnapshot<TopOfBook>, 3> &book_snapshots, std::array<SharedSnapshot<PairInfo>, 3> &info_snapshots) noexcept :
  _book_snapshots(book_snapshots),
  _info_snapshots(info_snapshots),
  _book_versions{0},
  _info_versions{0},
  _pairs(pairs) {}

COLD void ArbitrageScanner::start(void)
{
  getPairInfo();
  initBooks();

  while (true)
  {
    #pragma GCC unroll 3
    for (uint8_t i = 0; i < 3; ++i)
    {
      _book_snapshots[i].try_pop(_books[i], _book_versions[i]);
      checkArbitrage(_books);
    }
  }

  std::unreachable();
}

COLD void ArbitrageScanner::getPairInfo(void)
{
  int8_t combined_exponent = 0;

  std::array<size_t, 3> info_versions{0};

  for (uint8_t i = 0; i < 3; ++i)
  {
    PairInfo info{};

    while (!_info_snapshots[i].try_pop(info, info_versions[i]))
      std::this_thread::yield();

    _price_exponents[i] = info.price_exponent;
    _qty_exponents[i] = info.qty_exponent;
    combined_exponent += info.price_exponent;
  }

  static constexpr double LOG2_10 = 3.3219280948873626;
  _price_threshold = price_type(combined_exponent * LOG2_10);
}

COLD void ArbitrageScanner::initBooks(void)
{
  do
  {
    for (uint8_t i = 0; i < 3; ++i)
      _book_snapshots[i].try_pop(_books[i], _book_versions[i]);
  }
  while (
    (_book_versions[0] == 0) |
    (_book_versions[1] == 0) |
    (_book_versions[2] == 0)
  );
}

//TODO remove
#include <iostream>
HOT void ArbitrageScanner::checkArbitrage(const std::array<TopOfBook, 3> &books)
{
  const TopOfBook &tob0 = books[0];
  const TopOfBook &tob1 = books[1];
  const TopOfBook &tob2 = books[2];

  fast_assert(
    (tob0.bid_price > 0) & (tob0.ask_price > 0) &
    (tob1.bid_price > 0) & (tob1.ask_price > 0) &
    (tob2.bid_price > 0) & (tob2.ask_price > 0)
  );

  static constexpr auto log2 = [](const int64_t price) -> price_type {
    return price_type::log2(static_cast<uint64_t>(price));
  };

  //TODO remove
  for(int i = 0; i < 100; ++i)
  {
    price_type res = log2(i);
    std::cout << "log2(" << i << ") = " << res << "\n";
  }

  exit(1);

  const price_type path1 = log2(tob0.bid_price) + log2(tob1.ask_price) + log2(tob2.bid_price); // A→B→C→A
  const price_type path2 = log2(tob0.ask_price) + log2(tob1.bid_price) + log2(tob2.ask_price); // A←B←C←A

  const bool is_arbitrage = (path1 > _price_threshold) | (path2 > _price_threshold);

  //TODO remove
  if (is_arbitrage)
  {
    std::cout << "Arbitrage opportunity detected:\n"
              << "Bid/Ask Prices:\n"
              << "  Pair 0: Bid = " << tob0.bid_price << ", Ask = " << tob0.ask_price << "\n"
              << "  Pair 1: Bid = " << tob1.bid_price << ", Ask = " << tob1.ask_price << "\n"
              << "  Pair 2: Bid = " << tob2.bid_price << ", Ask = " << tob2.ask_price << "\n";
  }

  //TODO check quantity

  //TODO execute arbitrage
}