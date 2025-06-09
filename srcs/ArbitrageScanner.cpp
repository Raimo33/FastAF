/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-09 16:10:34                                                

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

COLD ArbitrageScanner::ArbitrageScanner(const std::array<currency_pair, 3> &pairs) :
  _mem_names{
    "binance_" + pairs[0].first + pairs[0].second,
    "binance_" + pairs[1].first + pairs[1].second,
    "binance_" + pairs[2].first + pairs[2].second
  },
  _queue_fds{
    shm_open(_mem_names[0].c_str(), O_RDONLY, 0666),
    shm_open(_mem_names[1].c_str(), O_RDONLY, 0666),
    shm_open(_mem_names[2].c_str(), O_RDONLY, 0666)
  },
  _queues{
    queue_type(_queue_fds[0]),
    queue_type(_queue_fds[1]),
    queue_type(_queue_fds[2])
  }
{
}

COLD ArbitrageScanner::~ArbitrageScanner(void)
{
  for (const int fd : _queue_fds)
    close(fd);
}

COLD void ArbitrageScanner::start(void)
{
  getFirstMessages();

  while (true)
  {
    #pragma GCC unroll 3
    for (uint8_t i = 0; i < 3; ++i)
      checkArbitrage(!_queues[i].pop(_last_messages[i]));
  }

  std::unreachable();
}

COLD void ArbitrageScanner::getFirstMessages(void)
{
  uint8_t combined_exponent = 0;

  for (uint8_t i = 0; i < 3; ++i)
  {
    while (!_queues[i].pop(_last_messages[i]))
      std::this_thread::yield();

    InternalMessage::PairInfo &pair_info = _last_messages[i].pair_info;

    _price_exponents[i] = pair_info.price_exponent;
    _qty_exponents[i] = pair_info.qty_exponent;
    combined_exponent += pair_info.price_exponent;
  }

  static constexpr double LOG2_10 = 3.3219280948873626;
  _price_threshold = price_type(combined_exponent * LOG2_10);
}

HOT void ArbitrageScanner::checkArbitrage(const bool no_op)
{
  if (no_op) [[unlikely]] //TODO branchless
    return;

  const InternalMessage::TopOfBook &tob0 = _last_messages[0].top_of_book;
  const InternalMessage::TopOfBook &tob1 = _last_messages[1].top_of_book;
  const InternalMessage::TopOfBook &tob2 = _last_messages[2].top_of_book;

  bool is_arbitrage = false;

  constexpr auto log2 = [](auto x) {
    return price_type::log2(x);
  };

  const price_type path1 = log2(tob0.bid_price) + log2(tob1.ask_price) + log2(tob2.bid_price); // A→B→C→A
  const price_type path2 = log2(tob0.ask_price) + log2(tob1.bid_price) + log2(tob2.ask_price); // A←B←C←A

  is_arbitrage = (path1 > _price_threshold) | (path2 > _price_threshold);

  if (is_arbitrage)
    printf("Arbitrage detected!\n");
}