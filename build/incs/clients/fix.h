/*================================================================================

File: fix.h                                                                     
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef FIX_H
# define FIX_H

# include <stdint.h>

# include "primitives/network_primitives.h"
# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "parsing/fix_serializer.h"
# include "fixed_point.h"
# include "graph.h"

# define FIX_HOST "fix-oe.testnet.binance.vision" //"fix-oe.binance.com"
# define FIX_PORT "9000"
# define FIX_KEEPALIVE_IDLE 300
# define FIX_KEEPALIVE_INTVL 30
# define FIX_KEEPALIVE_CNT 3
# define FIX_READ_BUFFER_SIZE 4096
# define FIX_WRITE_BUFFER_SIZE 4096
# define FIX_VERSION "FIX.4.4"
# define FIX_COMP_ID "FAF42"
# define FIX_HEARTBEAT_INTERVAL "60"

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api#message-components
//https://github.com/binance/binance-spot-api-docs/blob/master/fix-api.md
// https://github.com/binance/binance-spot-api-docs/blob/master/fix/schemas/spot-fix-oe.xml

typedef struct
{
  int sock_fd;
  struct sockaddr_in addr;
  SSL *ssl;
  keys_t *keys;
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  ff_message_t fix_message;
  uint32_t write_offset;
  uint32_t read_offset;
  uint64_t msg_seq_num;
  client_status_t status;
} fix_client_t;

COLD void init_fix(fix_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_fix_connection(const int fd, const uint32_t events, void *data);
HOT void handle_fix_setup(const int fd, const uint32_t events, void *data);
HOT void handle_fix_trading(const int fd, const uint32_t events, void *data);
COLD void free_fix(fix_client_t *restrict client);

#endif
