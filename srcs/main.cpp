#include "BinanceClient.hpp"
#include "utils.hpp"

#include <string>

int main(void)
{
  const char *api_key = std::getenv("BINANCE_API_KEY");
  if (!api_key)
    return 1;

  BinanceClient client("btcusdt", api_key);

  client.start();
}