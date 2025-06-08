/*================================================================================

File: ArbitrageScanner.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "macros.hpp"

#include <utility>

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
  while (true)
  {
    #pragma GCC unroll 3
    for (uint8_t i = 0; i < 3; ++i)
      checkArbitrage(_queues[i].pop(_tops_of_book[i]));
  }

  std::unreachable();
}

HOT void ArbitrageScanner::checkArbitrage(const bool no_op)
{
  if (no_op) [[unlikely]] //TODO branchless
    return;


  //scale prices to the max exponent of all (cached)
}

