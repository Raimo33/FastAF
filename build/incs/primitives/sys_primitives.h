/*================================================================================

File: sys_primitives.h                                                          
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef SYS_PRIMITIVES_H
# define SYS_PRIMITIVES_H

# include <stdint.h>
# include <sys/resource.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int32_t setrlimit_p(int32_t resource, const struct rlimit *rlim);

#endif