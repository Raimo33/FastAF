/*================================================================================

File: errors.c                                                                  
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "errors.h"

# include <stdio.h> //TODO remove

inline void fast_assert(const bool condition, const char *restrict msg)
{
  if (UNLIKELY(!condition))
    panic(msg);
}

inline void panic(const char *restrict msg)
{
  fflush(stderr); //TODO remove
  fflush(stdout); //TODO remove
  exit(EXIT_FAILURE);
}

void wait_child_process(const pid_t pid)
{
  int32_t status;

  waitpid(pid, &status, 0);
  if (WIFEXITED(status))
  {
    const int8_t exit_status = WEXITSTATUS(status);
    if (exit_status != EXIT_SUCCESS)
      exit(exit_status);
  }
  else
    exit(EXIT_FAILURE);
}