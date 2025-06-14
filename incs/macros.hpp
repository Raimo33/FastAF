/*================================================================================

File: macros.h                                                                  
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-08 18:21:38                                                 
last edited: 2025-06-14 19:36:05                                                

================================================================================*/

#pragma once

#define HOT                       __attribute__((hot))
#define COLD                      __attribute__((cold))
#define ALWAYS_INLINE             __attribute__((always_inline))
#define NEVER_INLINE              __attribute__((noinline))
#define PURE                      __attribute__((pure))
#define UNUSED                    __attribute__((unused))
#define PREFETCH_R(x, priority)   __builtin_prefetch(x, 0, priority)
#define PREFETCH_W(x, priority)   __builtin_prefetch(x, 1, priority)
#define restrict                  __restrict__
#define CACHELINE_SIZE           64

#include <utility>

HOT static inline void fast_assert(const bool condition)
{
  __asm__ __volatile__(
    "test %[cond], %[cond]\n\t"

    "lea 1f(%%rip), %%rax\n\t"
    "lea 2f(%%rip), %%rdx\n\t"

    "cmovz %%rdx, %%rax\n\t" 
    "jmp *%%rax\n\t"

    "1:\n\t"
    "ret\n\t"
    "2:\n\t"
    "ud2\n\t"
    :
    : [cond] "r"(condition)
    : "rax", "rdx"
  );

  std::unreachable();
}

/* unsafe approach

static inline void fast_assert(const bool condition) {
  *(volatile void *)condition;
}

*/