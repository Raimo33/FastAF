/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-09 12:36:36                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "macros.hpp"

#include <utility>
#include <thread>

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
  },
  _combined_price_exponent(0)
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

  _price_scale_factor = std::pow(10, combined_exponent);
}

HOT void ArbitrageScanner::checkArbitrage(const bool no_op)
{
  if (no_op) [[unlikely]] //TODO branchless
    return;

  const InternalMessage::TopOfBook &tob0 = _last_messages[0].top_of_book;
  const InternalMessage::TopOfBook &tob1 = _last_messages[1].top_of_book;
  const InternalMessage::TopOfBook &tob2 = _last_messages[2].top_of_book;

  bool is_arbitrage = false;

  static constexpr auto mul = [](uint64_t a, uint64_t b, uint64_t c) -> __uint128_t {
    return (__uint128_t)a * b * c;
  };

  //simdify
  const __uint128_t product1 = mul(tob0.bid_price, tob1.ask_price, tob2.bid_price);
  const __uint128_t product2 = mul(tob2.bid_price, tob1.ask_price, tob0.bid_price);
  const __uint128_t product3 = mul(tob2.bid_price, tob0.ask_price, tob1.bid_price);
  const __uint128_t product4 = mul(tob1.bid_price, tob2.ask_price, tob0.bid_price);
  const __uint128_t product5 = mul(tob2.bid_price, tob0.ask_price, tob1.bid_price);
  const __uint128_t product6 = mul(tob1.bid_price, tob0.ask_price, tob2.bid_price);

  //simdify
  is_arbitrage = (product1 + product2 + product3 + product4 + product5 + product6) > 6 * _price_scale_factor;

}

