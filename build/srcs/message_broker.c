/*================================================================================

File: message_broker.c                                                          
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "message_broker.h"

bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset)
{
  const uint32_t ret = SSL_write_p(ssl, buffer + *offset, len);
  if (UNLIKELY(ret <= 0))
    return false;
  
  *offset += ret;
  if (*offset != len)
    return false;

  bzero(buffer, len);
  *offset = 0;
  return true;
}

//TODO reduce dereferences
bool try_ssl_recv_http(http_client_t *client, http_response_t *restrict http_response)
{
  char *const buffer = client->read_buffer;

  const uint32_t ret = SSL_read_p(client->ssl, buffer + client->read_offset, HTTP_READ_BUFFER_SIZE - client->read_offset);
  if (UNLIKELY(ret <= 0))
    return false;

  client->read_offset += ret;
  fast_assert(client->read_offset <= buffer_size, "Response too big");
  if (LIKELY(!http1_is_complete(buffer, client->read_offset)))
    return false;

  const uint32_t bytes_deserialized = http1_deserialize(buffer, buffer_size, http_response);
  memmove(buffer, buffer + bytes_deserialized, client->read_offset - bytes_deserialized);
  client->read_offset -= bytes_deserialized;

  return true;
}

bool try_ssl_recv_fix(fix_client_t *client, fix_message_t *restrict fix_message)
{
  char *const buffer = client->read_buffer;

  const uint32_t ret = SSL_read_p(client->ssl, buffer + client->read_offset, FIX_READ_BUFFER_SIZE - client->read_offset);
  if (UNLIKELY(ret <= 0))
    return false;

  client->read_offset += ret;
  fast_assert(client->read_offset <= buffer_size, "Response too big");
  if (LIKELY(!ff_is_complete(buffer, client->read_offset)))
    return false;

  const uint32_t bytes_deserialized = ff_deserialize(buffer, buffer_size, fix_message);
  memmove(buffer, buffer + bytes_deserialized, client->read_offset - bytes_deserialized);
  client->read_offset -= bytes_deserialized;

  return true;
}