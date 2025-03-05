/*================================================================================

File: time_primitives.c                                                         
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "primitives/time_primitives.h"

# include <stdio.h> //TODO remove

inline time_t time_p(time_t *tloc)
{
  const time_t ret = time(tloc);
  if (ret == (time_t)-1)
  {
    printf("DEB TIME ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline struct tm *gmtime_r_p(const time_t *timep, struct tm *result)
{
  struct tm *ret = gmtime_r(timep, result);
  if (ret == NULL)
  {
    printf("DEB GMTIME_R ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}