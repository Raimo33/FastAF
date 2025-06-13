/*================================================================================

File: utils.cpp                                                                 
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-13 18:01:03                                                

================================================================================*/

#include "utils.hpp"
#include "macros.hpp"

#include <iostream>
#include <sched.h>

namespace boost
{

#ifndef _cpp_exceptions
COLD NEVER_INLINE void throw_exception(const std::exception &e)
{
  std::cerr << "Exception: " << e.what() << std::endl;
  std::abort();
}

COLD NEVER_INLINE void throw_exception(const std::exception &e, const boost::source_location &loc)
{
  std::cerr << loc << ": " << e.what() << std::endl;
  std::abort();
}
#endif

} // namespace boost

namespace utils
{

COLD NEVER_INLINE void throw_error(std::string_view message)
{
#ifdef __cpp_exceptions
  throw std::runtime_error(message.data());
#else
  std::cerr << "Error: " << message << std::endl;
  std::abort();
#endif
}

COLD std::pair<std::string, std::string> parse_pair(std::string_view pair)
{
  const size_t pos = pair.find('/');
  
  std::string base(pair.substr(0, pos));
  std::string quote(pair.substr(pos + 1));

  if (base.empty() | quote.empty())
    throw_error("Invalid pair format: " + std::string(pair));

  return {std::move(base), std::move(quote)};
}

COLD void pin_thread_to_core(std::jthread &thread, const int core_id)
{
  if (core_id < 0)
    throw_error("Invalid core ID: " + std::to_string(core_id));

  const auto handle = thread.native_handle();
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  if (pthread_setaffinity_np(handle, sizeof(cpu_set_t), &cpuset) != 0)
    throw_error("Failed to set thread affinity to core " + std::to_string(core_id));
}

} // namespace utils
