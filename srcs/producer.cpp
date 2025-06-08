/*================================================================================

File: producer.cpp                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 13:31:29                                                

================================================================================*/

#include "BinanceClient.hpp"
#include "utils.hpp"

int main(int argc, char **argv)
{
  if (argc != 2)
    utils::throw_error("Usage: " + std::string(argv[0]) + " <pair>");

  const char *api_key = std::getenv("BINANCE_API_KEY");
  if (!api_key)
    utils::throw_error("missing environment variable: BINANCE_API_KEY");

  const char *pair = argv[1];
  const std::string mem_name = "/binance_" + std::string(pair) + "_queue";

  const int queue_fd = shm_open(mem_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);

  BinanceClient client(pair, api_key, queue_fd);
  client.start();

  close(queue_fd);
  shm_unlink(pair);
}