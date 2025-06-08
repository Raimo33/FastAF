/*================================================================================

File: ArbitrageFinder.cpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#include "ArbitrageFinder.hpp"
#include "macros.hpp"

COLD ArbitrageFinder::ArbitrageFinder(std::array<int, 3> queue_fds) noexcept :
  _queues{
    queue_type(queue_fds[0]),
    queue_type(queue_fds[1]),
    queue_type(queue_fds[2])
  },
  _common_price_exponent(0)
{

}

COLD void ArbitrageFinder::start(void)
{
  messages::InternalMessage message;

  while (true)
  {
    #pragma unroll
    for (uint8_t i = 0; i < 3; ++i)
    {
      if (!_queues[i].pop(message)) [[likely]]
        continue;

      processMessage(message, i);
      checkArbitrage();
    }
  }

  std::unreachable();
}

HOT void ArbitrageFinder::processMessage(const messages::InternalMessage &message, const uint8_t pair_id)
{
  switch (message.type)
  {
    case messages::InternalMessage::TopOfBook:
      handleTopOfBook(message.top_of_book, pair_id);
      break;
    case messages::InternalMessage::PairInfo:
      handlePairInfo(message.pair_info, pair_id);
      break;
    default:
      break;
  }
}

COLD void ArbitrageFinder::handlePairInfo(const messages::InternalMessage::PairInfo &pair_info, const uint8_t pair_id)
{
  _price_exponents[pair_id] = pair_info.price_exponent;
  _qty_exponents[pair_id] = pair_info.qty_exponent;
  _common_price_exponent = std::max(_common_price_exponent, pair_info.price_exponent);
}

HOT void ArbitrageFinder::handleTopOfBook(const messages::InternalMessage::TopOfBook &top_of_book, const uint8_t pair_id)
{
  _prices_graph.setWeigh(/*currency idx0*/, /*currency idx1*/, adjusted_price);
  _prices_graph.setWeigh(/*currency idx1*/, /*currency idx0*/, inverse_adjusted_price);
}
