/*================================================================================

File: fastaf.h                                                                  
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef FASTAF_H
# define FASTAF_H

# include "errors.h"
# include "signals.h"
# include "clients/fix.h"
# include "clients/ws.h"
# include "clients/http.h"
# include "event_loop.h"
# include "keys.h"
# include "fast_ssl.h"
# include "graph.h"

typedef struct
{
  keys_t keys;
  SSL_CTX *ssl_ctx;
  int epoll_fd;
  clients_t clients;
  int sig_fd;
  graph_t graph;
} app_resources_t;

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//TODO SSL_shutdown() //attenzione al non-blocking
//TODO ebpf

#endif