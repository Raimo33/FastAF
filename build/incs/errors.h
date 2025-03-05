/*================================================================================

File: errors.h                                                                  
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef ERRORS_H
# define ERRORS_H

# include <stdint.h>

# include "extensions.h"
# include "errno.h"
# include "stdlib.h"
# include "sys/wait.h"

HOT ALWAYS_INLINE extern inline void fast_assert(const bool condition, const char *restrict msg);
NORETURN COLD extern inline void panic(const char *restrict msg);
HOT void wait_child_process(const pid_t pid);

#endif