/*================================================================================

File: time_primitives.h                                                         
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef TIME_PRIMITIVES_H
# define TIME_PRIMITIVES_H

# include <stdint.h>
# include <time.h>

# include "extensions.h"
# include "errors.h"

HOT extern inline time_t time_p(time_t *tloc);
HOT extern inline struct tm *gmtime_r_p(const time_t *timep, struct tm *result);

#endif