#include "BinanceClient.hpp"

#include <string>

int main(void)
{
  const std::string api_key = std::getenv("BINANCE_API_KEY");

  BinanceClient client("btcusdt", api_key);

  client.start();
}