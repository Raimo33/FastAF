#pragma once

#include "macros.hpp"

#include <boost/exception/exception.hpp>

#ifndef __EXCEPTIONS
COLD NEVER_INLINE void boost::throw_exception(UNUSED const std::exception &e)
{
  std::abort();
}

COLD NEVER_INLINE void boost::throw_exception(UNUSED const std::exception &e, UNUSED const boost::source_location &loc)
{
  std::abort();
}
#endif