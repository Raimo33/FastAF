/*================================================================================

File: http.h                                                                    
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef HTTP_H
# define HTTP_H

# include <stdint.h>

# include "primitives/network_primitives.h"
# include "primitives/yyjson_primitives.h"
# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "parsing/decompression.h"
# include "graph.h"

// # define HTTP_HOST "api.binance.com"
# define HTTP_HOST "testnet.binance.vision"
# define HTTP_PORT "443"
# define HTTP_READ_BUFFER_SIZE 4096
# define HTTP_WRITE_BUFFER_SIZE 4096
# define HTTP_KEEPALIVE_IDLE  5
# define HTTP_KEEPALIVE_INTVL 1
# define HTTP_KEEPALIVE_CNT   3

typedef struct
{
  int sock_fd;
  struct sockaddr_in addr;
  SSL *ssl;
  keys_t *keys;
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  client_status_t status;
} http_client_t;

COLD void init_http(http_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_http_connection(const int fd, const uint32_t events, void *data);
HOT void handle_http_setup(const int fd, const uint32_t events, void *data);
HOT void handle_http_trading(const int fd, const uint32_t events, void *data);
COLD void free_http(http_client_t *restrict client);

#endif
