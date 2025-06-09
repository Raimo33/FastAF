/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-09 20:07:26                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "macros.hpp"

#include <utility>
#include <thread>
#include <cassert>

/*

p1 * e1 + p2 * e2 + p3 * e3 > 1
p1 * p1 * p3 > 10^(e1 + e2 + e3)
log2(p1) + log2(p2) + log2(p3) > log2(10^(e1 + e2 + e3))
log2(p1) + log2(p2) + log2(p3) > (e1 + e2 + e3) * log2(10)

*/

COLD void ArbitrageScanner::start(void)
{
  getPairInfo();

  std::array<TopOfBook, 3> books;

  while (true)
  {
    #pragma GCC unroll 3
    for (uint8_t i = 0; i < 3; ++i)
      _book_snapshots[i].load(books[i]);

    checkArbitrage(books);
  }

  std::unreachable();
}

COLD void ArbitrageScanner::getPairInfo(void)
{
  int8_t combined_exponent = 0;
  
  for (uint8_t i = 0; i < 3; ++i)
  {
    PairInfo info;

    while (!_info_snapshots[i].load(info))
      std::this_thread::yield();

    _price_exponents[i] = info.price_exponent;
    _qty_exponents[i] = info.qty_exponent;
    combined_exponent += info.price_exponent;
  }

  static constexpr double LOG2_10 = 3.3219280948873626;
  _price_threshold = price_type(combined_exponent * LOG2_10);
}

//TODO remove
#include <iostream>
HOT void ArbitrageScanner::checkArbitrage(const std::array<TopOfBook, 3> &books)
{
  const TopOfBook &tob0 = books[0];
  const TopOfBook &tob1 = books[1];
  const TopOfBook &tob2 = books[2];

  //TODO remove
  std::cout << "prices bid/ask: "
            << tob0.bid_price << "/" << tob0.ask_price << ", "
            << tob1.bid_price << "/" << tob1.ask_price << ", "
            << tob2.bid_price << "/" << tob2.ask_price << "\n";

  assert(
    (tob0.bid_price > 0) & (tob0.ask_price > 0) &
    (tob1.bid_price > 0) & (tob1.ask_price > 0) &
    (tob2.bid_price > 0) & (tob2.ask_price > 0)
  );

  static constexpr auto log2 = [](const int64_t price) -> price_type {
    return price_type::log2(static_cast<uint64_t>(price));
  };

  const price_type path1 = log2(tob0.bid_price) + log2(tob1.ask_price) + log2(tob2.bid_price); // A→B→C→A
  const price_type path2 = log2(tob0.ask_price) + log2(tob1.bid_price) + log2(tob2.ask_price); // A←B←C←A

  const bool is_arbitrage = (path1 > _price_threshold) | (path2 > _price_threshold);

  //TODO remove
  if (is_arbitrage)
  {
    std::cout << "Arbitrage opportunity detected:\n"
              << "Path 1: " << path1 << " > " << _price_threshold << "\n"
              << "Path 2: " << path2 << " > " << _price_threshold << "\n"
              << "Bid/Ask Prices:\n"
              << "  Pair 0: Bid = " << tob0.bid_price << ", Ask = " << tob0.ask_price << "\n"
              << "  Pair 1: Bid = " << tob1.bid_price << ", Ask = " << tob1.ask_price << "\n"
              << "  Pair 2: Bid = " << tob2.bid_price << ", Ask = " << tob2.ask_price << "\n";
    exit(EXIT_SUCCESS);
  }

  //TODO check quantity

  //TODO execute arbitrage
}