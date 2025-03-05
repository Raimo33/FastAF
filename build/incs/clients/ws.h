/*================================================================================

File: ws.h                                                                      
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef WS_H
# define WS_H

# include <stdint.h>

# include "primitives/network_primitives.h"
# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "graph.h"

// # define WS_HOST "stream.binance.com"
# define WS_HOST "stream.testnet.binance.vision"
# define WS_PORT "9443"
# define WS_PATH "/stream?timeUnit=microsecond"
# define WS_READ_BUFFER_SIZE 4096
# define WS_WRITE_BUFFER_SIZE 4096
# define WS_KEEPALIVE_IDLE 300
# define WS_KEEPALIVE_INTVL 30
# define WS_KEEPALIVE_CNT 3

typedef struct
{
  int sock_fd;
  struct sockaddr_in addr;
  SSL *ssl;
  char conn_key[BASE64_ENCODED_SIZE(WS_KEY_SIZE) + 1];
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  client_status_t status;
} ws_client_t;

COLD void init_ws(ws_client_t *restrict client, SSL_CTX *restrict ssl_ctx);
HOT void handle_ws_connection(const int fd, const uint32_t events, void *data);
HOT void handle_ws_setup(const int fd, const uint32_t events, void *data);
HOT void handle_ws_trading(const int fd, const uint32_t events, void *data);
COLD void free_ws(ws_client_t *restrict client);

#endif