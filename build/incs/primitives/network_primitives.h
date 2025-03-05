/*================================================================================

File: network_primitives.h                                                      
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef NETWORK_PRIMITIVES_H
# define NETWORK_PRIMITIVES_H

# include <stdint.h>
# include <netdb.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int getaddrinfo_a_p(int mode, struct gaicb *list[], int ent, struct sigevent *sig);
COLD extern inline int connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif