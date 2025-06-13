/*================================================================================

File: ArbitrageScanner.hpp                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 18:58:46                                                 
last edited: 2025-06-13 18:01:03                                                

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
    ArbitrageScanner(const std::array<currency_pair, 3> &pairs, std::array<SharedSnapshot<TopOfBook>, 3> &book_snapshots, std::array<SharedSnapshot<PairInfo>, 3> &info_snapshots) noexcept;
    ~ArbitrageScanner() = default;

    void start(void);

  private:
    ArbitrageScanner(const ArbitrageScanner &) = delete;
    ArbitrageScanner(ArbitrageScanner &&) = delete;
    ArbitrageScanner &operator=(const ArbitrageScanner &) = delete;

    static constexpr float THRESHOLD_PERCENTAGE = 0.1f;

    void getPairInfo(void);
    void precomputeConstants(void);
    void initBooks(void);
    void checkArbitrage(void) noexcept;

    using fixed_type = FixedPoint<6>;

    std::array<SharedSnapshot<PairInfo>, 3> &_info_snapshots;

    SharedSnapshot<TopOfBook> &_book_snapshot0;
    SharedSnapshot<TopOfBook> &_book_snapshot1;
    SharedSnapshot<TopOfBook> &_book_snapshot2;

    TopOfBook _book0;
    TopOfBook _book1;
    TopOfBook _book2;

    size_t _book_version0;
    size_t _book_version1;
    size_t _book_version2;

    int8_t _combined_price_exponent;
    int8_t _combined_qty_exponent;

    fixed_type _forward_rhs;
    fixed_type _backward_rhs;
    //TODO array of order-execution streams
};