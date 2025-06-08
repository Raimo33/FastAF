/*================================================================================

File: ArbitrageFinder.hpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#pragma once

#include <array>

#include "ipq/SPSCQueue.hpp"
#include "messages/InternalMessages.hpp"

class ArbitrageFinder
{
  public:
    static constexpr size_t QUEUE_CAPACITY = 64;

    ArbitrageFinder(std::array<int, 3> queue_fds) noexcept;
    ~ArbitrageFinder() = default;

    void start(void);

  private:
    ArbitrageFinder(const ArbitrageFinder &) = delete;
    ArbitrageFinder &operator=(const ArbitrageFinder &) = delete;

    void processMessage(const messages::InternalMessage &message, const uint8_t pair_id);
    void handleTopOfBook(const messages::InternalMessage::TopOfBook &top_of_book, const uint8_t pair_id);
    void handlePairInfo(const messages::InternalMessage::PairInfo &pair_info, const uint8_t pair_id);
    void checkArbitrage(void) const;

    using queue_type = ipq::SPSCQueue<messages::InternalMessage, QUEUE_CAPACITY>;

    std::array<queue_type, 3> _queues;
    TriangularGraph _prices_graph;
    std::array<uint64_t, 3> _quantities;
    std::array<uint8_t, 3> _price_exponents;
    std::array<uint8_t, 3> _qty_exponents;
    uint8_t _common_price_exponent;
};