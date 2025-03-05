/*================================================================================

File: fixed_point.c                                                             
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "fixed_point.h"

//TODO considerare INT_MIN e INT_MAX come infiniti?
//TODO precision normalization? prendere la max tra l due?

static const int32_t powers_of_ten[] = {
  1,
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000,
  1000000000
};

inline fixed_point_t fast_add(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value + b.value, .precision = a.precision};
}

inline fixed_point_t fast_sub(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value - b.value, .precision = a.precision};
}

inline fixed_point_t fast_mul(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value * b.value, .precision = a.precision + b.precision};
}

inline fixed_point_t fast_div(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = (a.value * powers_of_ten[a.precision]) / b.value, .precision = a.precision};
}

inline fixed_point_t fast_log2(const fixed_point_t a)
{
  return (fixed_point_t){.value = log2(a.value), .precision = a.precision};
}
