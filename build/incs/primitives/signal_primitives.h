/*================================================================================

File: signal_primitives.h                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef SIGNAL_PRIMITIVES_H
# define SIGNAL_PRIMITIVES_H

# include <stdint.h>
# include <signal.h>
# include <sys/signalfd.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int32_t sigemptyset_p(sigset_t *set);
COLD extern inline int32_t sigaddset_p(sigset_t *set, int signum);
COLD extern inline int32_t sigprocmask_p(int how, const sigset_t *set, sigset_t *oldset);
COLD extern inline int signalfd_p(int fd, const sigset_t *mask, int flags);

#endif