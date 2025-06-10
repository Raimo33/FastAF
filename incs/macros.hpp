/*================================================================================

File: macros.h                                                                  
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-08 18:21:38                                                 
last edited: 2025-06-10 12:42:08                                                

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

//TODO division by zero? null dereference?
static inline void fast_assert(const bool condition) {
  __asm__ __volatile__(
    //TODO cmov
  );

pass:
  return;
fail:
  __asm__ __volatile__("ud2");
}