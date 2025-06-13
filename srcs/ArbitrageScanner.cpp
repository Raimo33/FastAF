/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-10 22:44:06                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "macros.hpp"

#include <utility>
#include <thread>

COLD ArbitrageScanner::ArbitrageScanner(const std::array<currency_pair, 3> &pairs, std::array<SharedSnapshot<TopOfBook>, 3> &book_snapshots, std::array<SharedSnapshot<PairInfo>, 3> &info_snapshots) noexcept :
  _book_snapshots(book_snapshots),
  _info_snapshots(info_snapshots),
  _book_versions{0},
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

  static constexpr double LOG_10 = std::log2(10.0);
  static constexpr double TAU = THRESHOLD_PERCENTAGE / 100.0;
  static constexpr double LOG_THRESHOLD = std::log2(1.0 + TAU);
  const double base_log_rhs = (-combined_exponent * LOG_10);
  _forward_rhs  = base_log_rhs + LOG_THRESHOLD;
  _backward_rhs = base_log_rhs - LOG_THRESHOLD;
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

  static constexpr auto log2 = [](const int64_t price) -> fixed_type {
    return fixed_type::log2(static_cast<uint64_t>(price));
  };

  //TODO FIX: the pair streams can be in whichever order
  const fixed_type forward_path  = log2(tob0.ask_price) + log2(tob1.ask_price) + log2(tob2.ask_price);
  const fixed_type backward_path = log2(tob0.bid_price) + log2(tob1.bid_price) + log2(tob2.ask_price);

  if (forward_path > _forward_rhs)
  {
    std::cout << "Forward arbitrage detected: " << forward_path << " > " << _forward_rhs << "\n";
  }
  else if (backward_path > _backward_rhs)
  {
    std::cout << "Backward arbitrage detected: " << backward_path << " > " << _backward_rhs << "\n";
  }

  //check max available quantity for arbitrage
  //execute arbitrage
}