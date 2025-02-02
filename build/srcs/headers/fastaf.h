/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastaf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:15:47 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 11:15:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FASTAF_H
# define FASTAF_H

# include "logger.h"
# include "errors.h"
# include "signals.h"
# include "fix.h"
# include "ws.h"
# include "rest.h"
# include "dns_resolver.h"
# include "event_loop.h"
# include "keys.h"
# include "fast_ssl.h"
# include "graph.h"

typedef struct
{
  keys_t keys;
  event_loop_ctx_t loop;
  SSL_CTX *ssl_ctx;
  clients_t clients;
  dns_resolver_t dns_resolver;
  graph_t graph;
} app_resources_t;

//TODO more defensive programming (assert everywhere)

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//TODO SSL_shutdown() //attenzione al non-blocking

#endif