/*================================================================================

File: signal_primitives.c                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "primitives/signal_primitives.h"

# include <stdio.h> //TODO remove

inline int sigemptyset_p(sigset_t *set)
{
  if (sigemptyset(set) == -1)
  {
    printf("DEB SIGEMPTYSET ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int sigaddset_p(sigset_t *set, int signum)
{
  if (sigaddset(set, signum) == -1)
  {
    printf("DEB SIGADDSET ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int sigprocmask_p(int how, const sigset_t *set, sigset_t *oldset)
{
  if (sigprocmask(how, set, oldset) == -1)
  {
    printf("DEB SIGPROCMASK ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int signalfd_p(int fd, const sigset_t *mask, int flags)
{
  const int ret = signalfd(fd, mask, flags);
  if (ret == -1)
  {
    printf("DEB SIGNALFD ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}