/*================================================================================

File: utils.cpp                                                                 
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-06-08 13:31:29                                                 
last edited: 2025-06-08 13:31:29                                                

================================================================================*/

#include "utils.hpp"
#include "macros.hpp"

#include <iostream>

#ifndef _cpp_exceptions
COLD NEVER_INLINE void boost::throw_exception(const std::exception &e)
{
  std::cerr << "Exception: " << e.what() << std::endl;
  std::abort();
}

COLD NEVER_INLINE void boost::throw_exception(const std::exception &e, const boost::source_location &loc)
{
  std::cerr << loc << ": " << e.what() << std::endl;
  std::abort();
}
#endif

COLD NEVER_INLINE void utils::throw_error(std::string_view message)
{
#ifdef __cpp_exceptions
  throw std::runtime_error(message.data());
#else
  std::cerr << "Error: " << message << std::endl;
  std::abort();
#endif
}
