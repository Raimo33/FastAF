/*================================================================================

File: alloc_primitives.c                                                        
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "primitives/alloc_primitives.h"

# include <stdio.h> //TODO remove

inline void *malloc_p(size_t size)
{
  void *ptr = malloc(size);
  if (ptr == NULL)
  {
    printf("DEB MALLOC ERROR\n");
    panic(strerror(errno));
  }
  return ptr;
}

inline void *calloc_p(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);
  if (ptr == NULL)
  {
    printf("DEB CALLOC ERROR\n");
    panic(strerror(errno));
  }
  return ptr;
}

inline void *realloc_p(void *ptr, size_t size)
{
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL)
  {
    printf("DEB REALLOC ERROR\n");
    panic(strerror(errno));
  }
  return new_ptr;
}