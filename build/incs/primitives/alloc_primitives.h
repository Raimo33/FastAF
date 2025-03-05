/*================================================================================

File: alloc_primitives.h                                                        
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef ALLOC_PRIMITIVES_H
# define ALLOC_PRIMITIVES_H

# include <stdint.h>
# include <stdlib.h>

# include "extensions.h"
# include "errors.h"

HOT extern inline void	*malloc_p(size_t size);
HOT extern inline void	*calloc_p(size_t nmemb, size_t size);
HOT extern inline void	*realloc_p(void *ptr, size_t size);

#endif