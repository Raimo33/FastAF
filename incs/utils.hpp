/*================================================================================

File: utils.hpp                                                                 
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 13:31:29                                                

================================================================================*/

#pragma once

#include <boost/exception/exception.hpp>
#include <string_view>
#include <string>
#include <thread>

#ifndef _cpp_exceptions
namespace boost
{
  void throw_exception(const std::exception &e);
  void throw_exception(const std::exception &e, const boost::source_location &loc);
}
#endif

namespace utils
{

void throw_error(std::string_view message);
std::pair<std::string, std::string> parse_pair(std::string_view pair);
void pin_thread_to_core(std::jthread &thread, const int core_id);

} // namespace utils