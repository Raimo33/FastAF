/*================================================================================

File: fix.c                                                                     
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "clients/fix.h"

COLD static bool send_logon_query(fix_client_t *restrict client);
COLD static bool receive_logon_response(fix_client_t *restrict client);
COLD static bool send_limits_query(fix_client_t *restrict client);
COLD static bool receive_limits_response(fix_client_t *restrict client);

void init_fix(fix_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx)
{
  const int fd = socket_p(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt_p(fd, IPPROTO_TCP, TCP_FASTOPEN, &(int32_t){5}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_NODELAY, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, SOL_SOCKET, SO_KEEPALIVE, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(int32_t){FIX_KEEPALIVE_IDLE}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(int32_t){FIX_KEEPALIVE_INTVL}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPCNT, &(int32_t){FIX_KEEPALIVE_CNT}, sizeof(int32_t));
 
  *client = (fix_client_t){
    .sock_fd = fd,
    .addr = {},
    .ssl = init_ssl_socket(fd, ssl_ctx, FIX_HOST),
    .keys = keys,
    .write_buffer = calloc_p(FIX_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc_p(FIX_READ_BUFFER_SIZE, sizeof(char)),
    .http_response = {},
    .fix_message = {},
    .write_offset = 0,
    .read_offset = 0,
    .msg_seq_num = 1,
  };
}

void handle_fix_connection(UNUSED const int fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&ssl_handshake, &&logon_query, &&logon_response, &&complete};
  static uint8_t sequence = 0;

  fix_client_t *client = data;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("FIX connection error");

  goto *states[sequence];

ssl_handshake:
  printf("Performing SSL handshake\n");
  if (!SSL_connect_p(client->ssl))
    return;
  sequence++;

logon_query:
  printf("Sending logon query\n");
  if (!send_logon_query(client))
    return;
  sequence++;

logon_response:
  printf("Receiving logon response\n");
  if (!receive_logon_response(client))
    return;
  sequence++;

complete:
  client->status = CONNECTED;
}

void handle_fix_setup(const int fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&limits_query, &&limits_response, &&complete};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("FIX connection error");

  fix_client_t *client = data;

  goto *states[sequence];

limits_query:
  printf("Sending limits query\n");
  if (!send_limits_query(client))
    return;
  sequence++;

limits_response:
  printf("Receiving limits response\n");
  if (!receive_limits_response(client)) //TODO salvare i limiti nella struttura client, ogni client ha i suoi limiti
    return;
  sequence++;

  (void)fd;

complete:
  client->status = TRADING;
}

void handle_fix_trading(const int fd, const uint32_t events, void *data)
{
  //TODO submit di ordini
  (void)fd;
  (void)data;
  (void)events;
  return;
}

static bool send_logon_query(fix_client_t *restrict client)
{
  static bool initialized;
  static uint16_t len;

  if (!initialized)
  {
    const char *timestamp_str = get_timestamp_utc_str();
    char seq_num_str[16];
    const uint8_t seq_num_str_len = ultoa(client->msg_seq_num, seq_num_str);

    const ff_message_t raw_data = {
      .tags = {FIX_MSGTYPE, FIX_SENDERCOMPID, FIX_TARGETCOMPID, FIX_MSGSEQNUM, FIX_SENDINGTIME},
      .values = {FIX_MSG_TYPE_LOGON, FIX_COMP_ID, "SPOT", seq_num_str, timestamp_str},
      .tag_lens = {STR_LEN(FIX_MSGTYPE), STR_LEN(FIX_SENDERCOMPID), STR_LEN(FIX_TARGETCOMPID), STR_LEN(FIX_MSGSEQNUM), STR_LEN(FIX_SENDINGTIME)},
      .value_lens = {STR_LEN(FIX_MSG_TYPE_LOGON), STR_LEN(FIX_COMP_ID), STR_LEN("SPOT"), seq_num_str_len, UTC_TIMESTAMP_SIZE},
      .n_fields = 5,
    };

    char serialized_data[1024];
    uint16_t data_len = ff_serialize_raw(serialized_data, &raw_data);
    
    char signed_data[ED25519_SIGSIZE];
    sign_ed25519(client->keys->priv_key, serialized_data, data_len, signed_data);
    data_len = sizeof(signed_data);
  
    char encoded_data[BASE64_ENCODED_SIZE(ED25519_SIGSIZE) + 1];
    data_len = EVP_EncodeBlock_p((uint8_t *)encoded_data, (const uint8_t *)signed_data, data_len);
  
    char data_len_str[16];
    const uint8_t data_len_str_len = ultoa(data_len, data_len_str);

    const ff_message_t message = {
      .tags = {FIX_MSGTYPE, FIX_SENDERCOMPID, FIX_TARGETCOMPID, FIX_MSGSEQNUM, FIX_SENDINGTIME, FIX_ENCRYPTMETHOD, FIX_HEARTBTINT, FIX_RAWDATALENGTH, FIX_RAWDATA, FIX_RESETSEQNUMFLAG, FIX_USERNAME, FIX_MESSAGEHANDLING, FIX_RESPONSEMODE},
      .values = {FIX_MSG_TYPE_LOGON, FIX_COMP_ID, "SPOT", seq_num_str, timestamp_str, "0", FIX_HEARTBEAT_INTERVAL, data_len_str, encoded_data, "Y", (char *)client->keys->api_key, "1", "1"},
      .tag_lens = {STR_LEN(FIX_MSGTYPE), STR_LEN(FIX_SENDERCOMPID), STR_LEN(FIX_TARGETCOMPID), STR_LEN(FIX_MSGSEQNUM), STR_LEN(FIX_SENDINGTIME), STR_LEN(FIX_ENCRYPTMETHOD), STR_LEN(FIX_HEARTBTINT), STR_LEN(FIX_RAWDATALENGTH), data_len_str_len, data_len, STR_LEN(FIX_RESETSEQNUMFLAG), API_KEY_SIZE, STR_LEN(FIX_MESSAGEHANDLING), STR_LEN(FIX_RESPONSEMODE)},
      .value_lens = {STR_LEN(FIX_MSG_TYPE_LOGON), STR_LEN(FIX_COMP_ID), STR_LEN("SPOT"), seq_num_str_len, UTC_TIMESTAMP_SIZE, STR_LEN("0"), STR_LEN(FIX_HEARTBEAT_INTERVAL), data_len_str_len, data_len, STR_LEN("Y"), API_KEY_SIZE, STR_LEN("1"), STR_LEN("1")},
      .n_fields = 13,
    };

    client->msg_seq_num++;
    len = serialize_fix_message(client->write_buffer, FIX_WRITE_BUFFER_SIZE, &message);
    len = finalize_fix_message(client->write_buffer, FIX_WRITE_BUFFER_SIZE, len);
  }

  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_logon_response(fix_client_t *restrict client)
{
  if (UNLIKELY(!try_ssl_recv_fix(client->ssl, client->read_buffer, FIX_READ_BUFFER_SIZE, &client->read_offset, &client->fix_message)))
    return false;
  
  //TODO check logon success
  (void)client;

  return false;
}

static bool send_limits_query(fix_client_t *restrict client)
{
  //TODO
  (void)client;
  return false;
}

static bool receive_limits_response(fix_client_t *restrict client)
{
  //TODO
  (void)client;
  return false;
}

UNUSED static void format_price(const fixed_point_t price, char *buffer)
{
  //TODO
  (void)price;
  (void)buffer;
}

void free_fix(fix_client_t *restrict client)
{
  close(client->sock_fd);
  
  if (UNLIKELY(client == NULL))
    return;

  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
}