/*================================================================================

File: producer.cpp                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 18:58:46                                                

================================================================================*/

#include "ArbitrageFinder.hpp"
#include "utils.hpp"

#include <string>
#include <array>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
  if (argc != 4)
    utils::throw_error("Usage: " + std::string(argv[0]) + " <pair> <pair> <pair>");

  std::array<std::string, 3> pairs;
  std::array<std::string, 3> mem_names;
  std::array<int, 3> queue_fds;

  for (int i = 0; i < 3; ++i)
  {
    pairs[i] = argv[i];
    mem_names[i] = "/binance_" + pairs[i];
    queue_fds[i] = shm_open(mem_names[i].c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
  }

  ArbitrageFinder finder(queue_fds);
  finder.start();

  for (int i = 0; i < 3; ++i)
  {
    close(queue_fds[i]);
    shm_unlink(mem_names[i].c_str());
  }
}
