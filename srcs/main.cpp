/*================================================================================

File: producer.cpp                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-14 19:36:05                                                

================================================================================*/

#include "MarketDataClient.hpp"
#include "ArbitrageScanner.hpp"
#include "utils.hpp"

int main(int argc, char **argv)
{
  if (argc != 4)
    utils::throw_error("Usage: " + std::string(argv[0]) + " <base/quote> <base/quote> <base/quote>");

  const char *api_key = std::getenv("BINANCE_API_KEY");
  if (!api_key)
    utils::throw_error("missing environment variable: BINANCE_API_KEY");

  std::array<currency_pair, 3> pairs = {
    utils::parse_pair(argv[1]),
    utils::parse_pair(argv[2]),
    utils::parse_pair(argv[3])
  };

  const bool correct_cycle = (pairs[0].second == pairs[1].first) & (pairs[1].second == pairs[2].second) & (pairs[2].first == pairs[0].first);
  if (!correct_cycle)
    utils::throw_error("Wrong cycle. Expected: A/B, B/C, A/C");

  std::array<SharedSnapshot<TopOfBook>, 3> book_snapshots;
  std::array<SharedSnapshot<PairInfo>, 3> info_snapshots;

  MarketDataClient client0(pairs[0], book_snapshots[0], info_snapshots[0], api_key);
  MarketDataClient client1(pairs[1], book_snapshots[1], info_snapshots[1], api_key);
  MarketDataClient client2(pairs[2], book_snapshots[2], info_snapshots[2], api_key);
  ArbitrageScanner scanner(pairs, book_snapshots, info_snapshots);

  std::jthread client_thread0([&client0]() { client0.start(); });
  std::jthread client_thread1([&client1]() { client1.start(); });
  std::jthread client_thread2([&client2]() { client2.start(); });
  std::jthread scanner_thread([&scanner]() { scanner.start(); });

  utils::pin_thread_to_core(client_thread0, 0);
  utils::pin_thread_to_core(client_thread1, 1);
  utils::pin_thread_to_core(client_thread2, 2);
  utils::pin_thread_to_core(scanner_thread, 3);
}