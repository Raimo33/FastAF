/*================================================================================

File: signals.c                                                                 
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "signals.h"

uint16_t init_signals(void)
{
  sigset_t mask;

  sigemptyset_p(&mask);
  sigaddset_p(&mask, SIGINT);
  sigaddset_p(&mask, SIGTERM);
  sigprocmask_p(SIG_BLOCK, &mask, NULL);

  return signalfd_p(-1, &mask, SFD_NONBLOCK);
}

void handle_signal(const int fd, UNUSED const uint32_t events, UNUSED void *data)
{
  struct signalfd_siginfo info;
  read_p(fd, &info, sizeof(info));

  switch (info.ssi_signo)
  {
    case SIGINT:
      FALLTHROUGH;
    case SIGTERM:
      panic("Received termination signal");
      UNREACHABLE;
  }
  UNREACHABLE;
}

void free_signals(const int fd)
{
  close(fd);
}