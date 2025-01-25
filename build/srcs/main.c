/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 11:48:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logger.h"
#include "headers/errors.h"
#include "headers/signals.h"
#include "headers/fix.h"
#include "headers/ws.h"
#include "headers/rest.h"
#include "headers/dns_resolver.h"
#include "headers/event_loop.h"
#include "headers/keys.h"
#include "headers/ssl.h"

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

/*
TODO FIXED POINTS

Replace division with precomputed reciprocals.

Vectorize with SIMD.

Inline assembly for critical paths.

Profile-guided and link-time optimizations.

*/

void *cleanup_label = NULL;

int32_t main(void)
{
  keys_t keys;
  event_loop_ctx_t loop;
  WOLFSSL_CTX *ssl_ctx;
  fix_client_t fix;
  ws_client_t ws;
  rest_client_t rest;
  dns_resolver_t resolver;
  cleanup_label = &&cleanup;

  init_logger();
  init_signals();
  init_keys(&keys);
  init_ssl(&ssl_ctx);
  init_fix(&fix, &keys, ssl_ctx);
  init_ws(&ws, ssl_ctx);
  init_rest(&rest, &keys, ssl_ctx);
  init_dns_resolver(&resolver);
  init_event_loop(&loop);

  establish_connections(&loop, &fix, &ws, &rest);
  listen_events(&loop, &fix, &ws, &rest);

cleanup:
  free_event_loop(&loop);
  free_dns_resolver(&resolver);
  free_rest(&rest);
  free_ws(&ws);
  free_fix(&fix);
  free_ssl(ssl_ctx);
  free_keys(&keys);
  free_signals();
  free_logger();  
}