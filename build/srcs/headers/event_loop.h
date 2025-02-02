/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:42:49 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 12:19:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_H
# define EVENT_LOOP_H

# include <sys/epoll.h>
# include <stdbool.h>

# include "extensions.h"
# include "signals.h"
# include "ws.h"
# include "fix.h"
# include "rest.h"
# include "dns_resolver.h"
# include "logger.h"
# include "graph.h"

# define MAX_EVENTS 128

# define SIGNAL_EVENTS  EPOLLIN | EPOLLONESHOT | EPOLLET
# define TCP_EVENTS     EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP | EPOLLERR | EPOLLET
# define UDP_EVENTS     EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR | EPOLLET
# define LOG_EVENTS     EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLET

typedef struct
{
  fix_client_t fix;
  ws_client_t ws;
  rest_client_t rest;
} clients_t;

typedef struct
{
  uint16_t epoll_fd;
    // Can add more event loop related state here
} event_loop_ctx_t;

COLD void init_event_loop(event_loop_ctx_t *restrict ctx);
COLD void connect_clients(const event_loop_ctx_t *restrict ctx, clients_t *restrict clients, dns_resolver_t *restrict dns_resolver);
COLD void setup_trading(const event_loop_ctx_t *restrict ctx, clients_t *restrict clients, graph_t *restrict graph);
COLD void trade(const event_loop_ctx_t *restrict ctx, clients_t *restrict clients, graph_t *restrict graph);
COLD void free_event_loop(const event_loop_ctx_t *restrict ctx);

#endif