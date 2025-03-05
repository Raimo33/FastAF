/*================================================================================

File: sys_primitives.c                                                          
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "primitives/sys_primitives.h"

# include <stdio.h> //TODO remove

inline int32_t setrlimit_p(int32_t resource, const struct rlimit *rlim)
{
  if (setrlimit(resource, rlim) == -1)
  {
    printf("DEB SETRLIMIT ERROR\n"); 
    panic(strerror(errno));
  }
  return 0;
}