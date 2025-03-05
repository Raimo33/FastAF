/*================================================================================

File: string_utils.h                                                            
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef STRING_UTILS_H
# define STRING_UTILS_H

# include <stdint.h>
# include <time.h>

# include "primitives/time_primitives.h"
# include "extensions.h"

# define STR_AND_LEN(s) s, sizeof(s) - 1
# define STR_LEN(s) sizeof(s) - 1
# define ARR_LEN(a) sizeof(a) / sizeof(a[0])
# define UTC_TIMESTAMP_SIZE STR_LEN("19700101-00:00:00")

HOT void strtolower(char *str, const uint16_t len);
HOT const char *get_timestamp_utc_str(void);
HOT uint8_t ultoa(uint64_t num, char *buffer);

#endif