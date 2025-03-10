/*================================================================================

File: http.c                                                                    
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "clients/http.h"

COLD static bool send_info_query(http_client_t *restrict client);
COLD static bool receive_info_response(http_client_t *restrict client);
COLD static void process_info_response(char *body, const uint32_t body_len);

void init_http(http_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx)
{

  const int fd = socket_p(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt_p(fd, IPPROTO_TCP, TCP_FASTOPEN, &(int32_t){5}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_NODELAY, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, SOL_SOCKET, SO_KEEPALIVE, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(int32_t){HTTP_KEEPALIVE_IDLE}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(int32_t){HTTP_KEEPALIVE_INTVL}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPCNT, &(int32_t){HTTP_KEEPALIVE_CNT}, sizeof(int32_t));

  *client = (http_client_t){
    .sock_fd = fd,
    .addr = {},
    .ssl = init_ssl_socket(fd, ssl_ctx, HTTP_HOST),
    .keys = keys,
    .write_buffer = calloc_p(HTTP_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc_p(HTTP_READ_BUFFER_SIZE, sizeof(char)),
    .http_response = {},
    .write_offset = 0,
    .read_offset = 0
  };
}

void handle_http_connection(UNUSED const int fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&ssl_handshake, &&complete};
  static uint8_t sequence = 0;

  http_client_t *client = data;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("HTTP connection error");

  goto *states[sequence];

ssl_handshake:
  printf("Performing SSL handshake\n");
  if (!SSL_connect_p(client->ssl))
    return;
  sequence++;

complete:
  client->status = CONNECTED;
}

void handle_http_setup(const int fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&info_query, &&info_response, &&complete};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("HTTP connection error");

  http_client_t *client = data;

  goto *states[sequence];

info_query:
  printf("Querying Exchange info\n");
  if (!send_info_query(client))
    return;
  sequence++;

info_response:
  printf("Received Exchange info\n");
  if (!receive_info_response(client))
    return;
  sequence++;

  //TODO altre chiamate per user data, limiti account ecc
  
  (void)fd;
complete:
  client->status = TRADING;
}

void handle_http_trading(const int fd, const uint32_t events, void *data)
{
  //TODO eventuali chiamate di http durante il trading
  (void)fd;
  (void)data;
  (void)events;
}

static bool send_info_query(http_client_t *restrict client)
{
  static bool initialized;
  static uint16_t len;

  if (!initialized)
  {
    const http_request_t request = {
      .method = GET,
      .path = "/api/v3/exchangeInfo?permissions=SPOT&showPermissionSets=false&symbolStatus=TRADING",
      .path_len = STR_LEN("/api/v3/exchangeInfo?permissions=SPOT&showPermissionSets=false&symbolStatus=TRADING"),
      .version = HTTP_1_1,
      .headers = (header_entry_t[]){
        { STR_AND_LEN("Host"), STR_AND_LEN(HTTP_HOST) },
        { STR_AND_LEN("Accept-Encoding"), STR_AND_LEN("gzip") },
        { STR_AND_LEN("Connection"), STR_AND_LEN("keep-alive") }
      },
      .n_headers = 3,
      .body = NULL,
      .body_len = 0
    };
    
    len = serialize_http_request(client->write_buffer, HTTP_WRITE_BUFFER_SIZE, &request);
    initialized = true;
  }

  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_info_response(http_client_t *restrict client)
{
  //TODO don't use the normal buffer, use a dedicated one for this response. (on the stack)

  if (UNLIKELY(!try_ssl_recv_http();
    return false;

  const http_response_t *restrict response = &client->http_response;
  fast_assert(response->status_code == 200, "Exchange info query failed: invalid status code");
  
  const header_entry_t *restrict content_encoding = header_map_get(&response->headers, STR_AND_LEN("content-encoding\n");
  fast_assert(content_encoding, "Exchange info query failed: missing content encoding header");
  fast_assert(!memcmp(content_encoding->value, STR_AND_LEN("gzip")), "Exchange info query failed: invalid content encoding");
  fast_assert(response->body, "Exchange info query failed: missing body");
  
  process_info_response(response->body, response->body_len);

  free_http_response(&client->http_response);
  client->read_buffer = realloc_p(client->read_buffer, HTTP_READ_BUFFER_SIZE);
  return true;
}

static void process_info_response(char *body, const uint32_t body_len)
{
  int32_t pipe_fds[2];
  pipe(pipe_fds);

  const pid_t pid = fork();
  if (pid == 0)
  {
    close(pipe_fds[0]);
    gzip_decompress_to_file((uint8_t *)body, body_len, (uint16_t)pipe_fds[1]);
    close(pipe_fds[1]);
    exit(EXIT_SUCCESS);
  }
  else
  {
    close(pipe_fds[1]);
    FILE *restrict fp = fdopen_p(pipe_fds[0], "rb");
    yyjson_doc *restrict doc = yyjson_read_fp_p(fp, 0, NULL, NULL);
    fclose(fp);
    //TODO processare il json
    yyjson_doc_free(doc);
    wait_child_process(pid);
  }
}

void free_http(http_client_t *restrict client)
{
  close(client->sock_fd);
  
  if (UNLIKELY(client == NULL))
    return;

  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
}