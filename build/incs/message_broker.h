/*================================================================================

File: message_broker.h                                                          
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef MESSAGE_BROKER_H
# define MESSAGE_BROKER_H

# include <stdint.h>
# include <arpa/inet.h>

# include "fast_ssl.h"
# include "extensions.h"

HOT bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset);
HOT bool try_ssl_recv_http(http_client_t *client, http_response_t *restrict http_response);
HOT bool try_ssl_recv_fix(fix_client_t *client, fix_message_t *restrict fix_message);

#endif