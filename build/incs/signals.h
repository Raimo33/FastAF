/*================================================================================

File: signals.h                                                                 
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdint.h>
# include <signal.h>
# include <sys/signalfd.h>

# include "primitives/signal_primitives.h"
# include "extensions.h"
# include "errors.h"

COLD uint16_t init_signals(void);
COLD void handle_signal(const int fd, const uint32_t events, void *data);
COLD void free_signals(int fd);

#endif