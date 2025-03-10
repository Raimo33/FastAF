/*================================================================================

File: extensions.h                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef EXTENSIONS_H
# define EXTENSIONS_H

# define LIKELY(x)                  __builtin_expect(!!(x), 1)
# define UNLIKELY(x)                __builtin_expect(!!(x), 0)
# define PREFETCH(x, rw, locality)  __builtin_prefetch(x, rw, locality)
# define PREFETCHW(x, locality)     __builtin_prefetch(x, 1, locality)
# define PREFETCHR(x, locality)     __builtin_prefetch(x, 0, locality)
# define UNREACHABLE                __builtin_unreachable()
# define FALLTHROUGH                __attribute__((fallthrough))
# define COLD                       __attribute__((cold))
# define HOT                        __attribute__((hot))
# define CONST                      __attribute__((const)) //TODO usare
# define PURE                       __attribute__((pure)) //TODO usare
# define UNUSED                     __attribute__((unused))
# define PACKED                     __attribute__((packed))
# define ALIGNED(x)                 __attribute__((aligned(x)))
# define NORETURN                   __attribute__((noreturn)) //TODO usare
# define ALWAYS_INLINE              __attribute__((always_inline))
# define NO_INLINE                  __attribute__((noinline))
# define FLATTEN                    __attribute__((flatten))
# define MALLOC                     __attribute__((malloc)) //TODO usare
# define NONNULL(...)               __attribute__((nonnull(__VA_ARGS__))) //TODO usare

# define L0   0
# define L1   1
# define L2   2
# define NTA  3

#endif