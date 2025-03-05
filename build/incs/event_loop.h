/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:42:49 by craimond          #+#    #+#             */
/*   Updated: 2025/02/08 13:42:47 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_H
# define EVENT_LOOP_H

# include <sys/epoll.h>


# include "primitives/file_primitives.h"
# include "primitives/network_primitives.h"
# include "extensions.h"
# include "signals.h"
# include "clients/ws.h"
# include "clients/fix.h"
# include "clients/http.h"

# include "graph.h"

# define MAX_EVENTS 128

# define SIGNAL_EVENTS  EPOLLIN | EPOLLONESHOT | EPOLLWAKEUP | EPOLLET
# define TCP_EVENTS     EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP | EPOLLERR | EPOLLET

typedef struct
{
  ws_client_t ws;
  fix_client_t fix;
  http_client_t http;
} clients_t;

typedef struct
{
  int epoll_fd;
  clients_t *clients;
  int sig_fd;
} event_loop_ctx_t;

typedef void (*EventHandler)(int fd, uint32_t events, void *data);

typedef struct
{
  EventHandler handler;
  void *data;
} HandlerEntry;

extern HandlerEntry handlers[MAX_FDS];

COLD uint8_t init_event_loop(clients_t *restrict clients, const int signal_fd);
COLD void resolve_domains(clients_t *restrict clients);
COLD void connect_clients(const int epoll_fd, clients_t *restrict clients, const int signal_fd);
COLD void setup_trading(const int epoll_fd, clients_t *restrict clients, const int signal_fd);
COLD void trade(const int epoll_fd, clients_t *restrict clients, const int signal_fd);
COLD void free_event_loop(const int epoll_fd);

#endif