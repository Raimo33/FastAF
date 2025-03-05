/*================================================================================

File: event_loop.c                                                              
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "event_loop.h"

HandlerEntry handlers[MAX_FDS] = {0};

HOT static inline uint8_t get_connected_clients(const clients_t *restrict clients);

int init_event_loop(clients_t *restrict clients, const int signal_fd)
{
  const int epoll_fd = epoll_create1_p(0);

  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, signal_fd, &(struct epoll_event) {
    .events = SIGNAL_EVENTS,
    .data = { .fd = signal_fd }
  });

  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, clients->ws.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->ws.sock_fd }
  });
  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, clients->fix.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->fix.sock_fd }
  });
  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, clients->http.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->http.sock_fd }
  });

  return epoll_fd;
}

void resolve_domains(clients_t *restrict clients)
{
  static const struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP
  };

  struct gaicb ws_request = {
    .ar_name = WS_HOST,
    .ar_service = WS_PORT,
    .ar_request = &hints
  };

  struct gaicb fix_request = {
    .ar_name = FIX_HOST,
    .ar_service = FIX_PORT,
    .ar_request = &hints
  };

  struct gaicb rest_request = {
    .ar_name = HTTP_HOST,
    .ar_service = HTTP_PORT,
    .ar_request = &hints
  };

  struct gaicb *requests[] = { &ws_request, &fix_request, &rest_request };

  getaddrinfo_a_p(GAI_WAIT, requests, ARR_LEN(requests), NULL);

  clients->ws.addr = *(struct sockaddr_in *)ws_request.ar_result->ai_addr;
  clients->fix.addr = *(struct sockaddr_in *)fix_request.ar_result->ai_addr;
  clients->http.addr = *(struct sockaddr_in *)rest_request.ar_result->ai_addr;

  freeaddrinfo(ws_request.ar_result);
  freeaddrinfo(fix_request.ar_result);
  freeaddrinfo(rest_request.ar_result);
}

void connect_clients(const int epoll_fd, clients_t *restrict clients, const int signal_fd)
{
  struct epoll_event events[MAX_EVENTS] ALIGNED(64) = {0};
  struct epoll_event *event;
  uint8_t n;
  int event_fd;

  handlers[signal_fd]             = (HandlerEntry){ handle_signal, NULL };
  handlers[clients->ws.sock_fd]   = (HandlerEntry){ handle_ws_connection, &clients->ws };
  handlers[clients->fix.sock_fd]  = (HandlerEntry){ handle_fix_connection, &clients->fix };
  handlers[clients->http.sock_fd] = (HandlerEntry){ handle_http_connection, &clients->http };

  connect_p(clients->ws.sock_fd, (struct sockaddr *)&clients->ws.addr, sizeof(struct sockaddr_in));
  connect_p(clients->fix.sock_fd, (struct sockaddr *)&clients->fix.addr, sizeof(struct sockaddr_in));
  connect_p(clients->http.sock_fd, (struct sockaddr *)&clients->http.addr, sizeof(struct sockaddr_in));

  while (LIKELY(get_connected_clients(clients) < 3))
  {
    n = epoll_wait_p(epoll_fd, events, MAX_EVENTS, -1);
    for (event = events; n > 0; n--, event++)
    {
      event_fd = event->data.fd;
      handlers[event_fd].handler(event_fd, event->events, handlers[event_fd].data);
    }
  }
}

void setup_trading(const int epoll_fd, clients_t *restrict clients, const int signal_fd)
{
  (void)epoll_fd;
  (void)clients;
  (void)signal_fd;
  //TODO fetches exchange info, fetches user info, fills graph and other
}

void trade(const int epoll_fd, clients_t *restrict clients, const int signal_fd)
{
  (void)epoll_fd;
  (void)clients;
  (void)signal_fd;
  //TODO: updates graph with live ws data, sends fix orders, heartbeats, periodical checks with http
}

static inline uint8_t get_connected_clients(const clients_t *restrict clients)
{
  return (clients->ws.status >= CONNECTED) + (clients->fix.status >= CONNECTED) + (clients->http.status >= CONNECTED);
}

void free_event_loop(const int epoll_fd)
{
  close(epoll_fd);
}