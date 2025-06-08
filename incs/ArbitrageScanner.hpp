/*================================================================================

File: ArbitrageScanner.hpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#pragma once

#include <cstdint>
#include <array>
#include <string>

#include "ipq/SPSCQueue.hpp"
#include "messages/InternalMessages.hpp"

using currency_pair = std::pair<std::string, std::string>;
using namespace messages::internal;

class ArbitrageScanner
{
  public:
    static constexpr size_t QUEUE_CAPACITY = 16;

    ArbitrageScanner(const std::array<currency_pair, 3> &pairs);
    ~ArbitrageScanner();

    void start(void);

  private:
    ArbitrageScanner(const ArbitrageScanner &) = delete;
    ArbitrageScanner &operator=(const ArbitrageScanner &) = delete;

    void checkArbitrage(const bool no_op);

    using queue_type = ipq::SPSCQueue<TopOfBook, QUEUE_CAPACITY>;

    std::array<std::string, 3> _mem_names;
    std::array<int, 3> _queue_fds;
    std::array<queue_type, 3> _queues;
    std::array<TopOfBook, 3> _tops_of_book;
};