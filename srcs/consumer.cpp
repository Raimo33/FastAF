/*================================================================================

File: producer.cpp                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#include "ArbitrageScanner.hpp"
#include "utils.hpp"

#include <string>
#include <array>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
  if (argc != 4)
    utils::throw_error("Usage: " + std::string(argv[0]) + " <base-quote> <base-quote> <base-quote>");

  std::array<currency_pair, 3> pairs;

  for (size_t i = 1; i < 4; ++i)
    pairs[i] = utils::parse_pair(argv[i]);

  ArbitrageScanner Scanner(pairs);
  Scanner.start();
}
