/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_broker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:17:03 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 21:00:32 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/message_broker.h"

bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset)
{
  const uint32_t ret = SSL_write(ssl, buffer + *offset, len);
  if (UNLIKELY(ret <= 0))
    return false;
  
  *offset += ret;
  if (*offset != len)
    return false;
  
  memset(buffer, 0, len);
  *offset = 0;
  return true;
}

bool try_ssl_recv_http(SSL *restrict ssl, char *restrict buffer, const uint32_t buffer_size, uint32_t *offset, http_response_t *restrict http_response)
{
  const uint32_t ret = SSL_read(ssl, buffer + *offset, buffer_size - *offset);
  if (UNLIKELY(ret <= 0))
    return false;

  *offset += ret;
  fast_assert(*offset < buffer_size, STR_LEN_PAIR("Response too big"));
  if (!is_full_http_response(buffer, buffer_size, *offset))
    return false;

  const uint32_t bytes_parsed = parse_http_response(buffer, http_response, buffer_size);
  memmove(buffer, buffer + bytes_parsed, *offset - bytes_parsed);
  *offset -= bytes_parsed;
  return true;
}
