/*================================================================================

File: ArbitrageScanner.hpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-09 20:09:32                                                

================================================================================*/

#pragma once

#include <cstdint>
#include <array>
#include <string>

#include "FixedPoint.hpp"
#include "SharedSnapshot.hpp"
#include "messages/InternalMessages.hpp"

using currency_pair = std::pair<std::string, std::string>;
using namespace messages::internal;

class ArbitrageScanner
{
  public:
    ArbitrageScanner(const std::array<currency_pair, 3> &pairs) = default;
    ~ArbitrageScanner() = default;

    void start(void);

  private:
    ArbitrageScanner(const ArbitrageScanner &) = delete;
    ArbitrageScanner(ArbitrageScanner &&) = delete;
    ArbitrageScanner &operator=(const ArbitrageScanner &) = delete;

    using price_type = FixedPoint<8, 24>;

    void getPairInfo(void);
    void checkArbitrage(const std::array<TopOfBook, 3> &books);

    std::array<SharedSnapshot<TopOfBook>, 3> _book_snapshots;
    std::array<SharedSnapshot<PairInfo>, 3> _info_snapshots;
    //execution stream (Binance client executor) (the pair names are available in the constructor in the correct order  )
    std::array<int8_t, 3> _price_exponents;
    std::array<int8_t, 3> _qty_exponents;
    price_type _price_threshold;
};