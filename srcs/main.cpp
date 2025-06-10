/*================================================================================

File: producer.cpp                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-10 12:42:08                                                

================================================================================*/

#include "MarketDataClient.hpp"
#include "ArbitrageScanner.hpp"
#include "utils.hpp"

int main(int argc, char **argv)
{
  if (argc != 4)
    utils::throw_error("Usage: " + std::string(argv[0]) + " <base-quote> <base-quote> <base-quote>");

  const char *api_key = std::getenv("BINANCE_API_KEY");
  if (!api_key)
    utils::throw_error("missing environment variable: BINANCE_API_KEY");

  std::array<currency_pair, 3> pairs;

  for (size_t i = 0; i < 3; ++i)
    pairs[i] = utils::parse_pair(argv[i + 1]);

  MarketDataClient client0(pairs[0], api_key);
  MarketDataClient client1(pairs[1], api_key);
  MarketDataClient client2(pairs[2], api_key);
  ArbitrageScanner scanner(pairs);

  //TODO should threads share the same io_context?
  std::jthread client_thread0([&client0]() { client0.start(); });
  std::jthread client_thread1([&client1]() { client1.start(); });
  std::jthread client_thread2([&client2]() { client2.start(); });
  std::jthread scanner_thread([&scanner]() { scanner.start(); });

  utils::pin_thread_to_core(client_thread0, 0);
  utils::pin_thread_to_core(client_thread1, 1);
  utils::pin_thread_to_core(client_thread2, 2);
  utils::pin_thread_to_core(scanner_thread, 3);
}