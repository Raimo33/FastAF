/*================================================================================

File: producer.cpp                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#include "BinanceClient.hpp"
#include "utils.hpp"

int main(int argc, char **argv)
{
  if (argc != 2)
    utils::throw_error("Usage: " + std::string(argv[0]) + " <base-quote>");

  const char *api_key = std::getenv("BINANCE_API_KEY");
  if (!api_key)
    utils::throw_error("missing environment variable: BINANCE_API_KEY");

  currency_pair pair = utils::parse_pair(argv[1]);

  BinanceClient client(pair, api_key);
  client.start();
}