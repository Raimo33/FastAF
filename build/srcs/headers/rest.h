/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 18:55:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REST_H
# define REST_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "ssl.h"

# define REST_HOST "api.binance.com"
# define REST_PORT 8080U
# define REST_FILENO 6U

# define REST_KEEPALIVE_IDLE  5U
# define REST_KEEPALIVE_INTVL 1U
# define REST_KEEPALIVE_CNT   3U

typedef struct
{
  struct sockaddr_in addr;
  ssl_sock_t ssl_sock;
  const keys_t *keys;
} rest_client_t;

void init_rest(rest_client_t *rest, const keys_t *keys);
bool handle_rest_connection_event(const rest_client_t *rest, const uint32_t events);
void handle_rest_event(const rest_client_t *rest);
void free_rest(const rest_client_t *rest);

#endif
