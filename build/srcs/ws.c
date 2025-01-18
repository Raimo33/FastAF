/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 22:25:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static uint8_t send_upgrade_request(const ws_client_t *ws);
static uint8_t receive_upgrade_response(const ws_client_t *ws);

//TODO pool di connessioni
void init_ws(ws_client_t *ws)
{
  ws->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(WS_PORT),
    .sin_addr = {
      .s_addr = inet_addr(WS_HOST) //TODO getaddrinfo e dns resolve (https://c-ares.org/docs.html)
    }
  };
  
  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));
  init_ssl_socket(fd, &ws->ssl_sock);

  dup2(fd, WS_FILENO);
  close(fd);
}

inline bool handle_ws_connection_event(const ws_client_t *ws, const char fd_state)
{
  static void *states[] = { &&connect, &&ssl_handshake, &&upgrade_request, &&upgrade_response };
  static uint8_t sequence;

  if (UNLIKELY(fd_state == 'e'))
    panic(STR_LEN_PAIR("Websocket connection error"));

  goto *states[sequence];

//TODO prefetching??

connect:
  connect(WS_FILENO, (struct sockaddr *)&ws->addr, sizeof(ws->addr));
  sequence++;
  return false;

ssl_handshake:
  if (wolfSSL_connect(ws->ssl_sock.ssl) == SSL_SUCCESS)
    sequence++;
  return false;

upgrade_request:
  if (send_upgrade_request(ws) > 0)
    sequence++;
  return false;

upgrade_response:
  if (receive_upgrade_response(ws) > 0)
    sequence++;
  return true;
}

static uint8_t send_upgrade_request(const ws_client_t *ws)
{
  static byte ws_key[WS_KEY_SIZE] ALIGNED(16);
  static http_request_t request ALIGNED(16) =
  {
    .method = HTTP_GET,
    .path = WS_PATH,
    .path_len = STR_LEN(WS_PATH),
    .version = HTTP_1_1,
    .headers_count = 5,
    .headers = {
      { STR_LEN_PAIR("Host"), STR_LEN_PAIR(WS_HOST) },
      { STR_LEN_PAIR("Upgrade"), STR_LEN_PAIR("websocket") },
      { STR_LEN_PAIR("Connection"), STR_LEN_PAIR("Upgrade") },
      { STR_LEN_PAIR("Sec-WebSocket-Version"), STR_LEN_PAIR("13") },
      { STR_LEN_PAIR("Sec-WebSocket-Key"), ws_key, WS_KEY_SIZE }
    }
    .body = NULL,
    .body_len = 0
  };
  static const uint16_t req_len = //TODO compute at compile time
  static byte raw_request[req_len] ALIGNED(16);

  if (//prima volta)
    generate_ws_key(ws_key);
    build_http_request(&request, raw_request);

  return wolfSSL_send(ws->ssl_sock.ssl, raw_request, req_len, MSG_NOSIGNAL);
}

static uint8_t receive_upgrade_response(const ws_client_t *ws)
{
  //base64 decode, 258EAFA5-E914-47DA-95CA-C5AB0DC85B11, sha1
  //parsing, controllo dello status code 101, controllo dell'header Sec-WebSocket-Accept etc
}

void free_ws(const ws_client_t *ws)
{
  free_ssl_socket(&ws->ssl_sock);
  close(WS_FILENO);
}