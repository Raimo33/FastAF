/*================================================================================

File: client_common.h                                                           
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef CLIENT_COMMON_H
# define CLIENT_COMMON_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "fast_ssl.h"
# include "message_broker.h"

typedef enum: uint8_t {DISCONNECTED, CONNECTED, TRADING} client_status_t;

#endif