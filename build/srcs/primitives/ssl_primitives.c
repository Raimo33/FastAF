/*================================================================================

File: ssl_primitives.c                                                          
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "primitives/ssl_primitives.h"

# include <stdio.h> //TODO remove

COLD static void handle_ssl_error(void);

inline int OPENSSL_init_ssl_p(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings)
{
  if (UNLIKELY(OPENSSL_init_ssl(opts, settings) == 0))
  {
    printf("DEB OPENSSL_INIT_SSL ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline SSL_CTX *SSL_CTX_new_p(const SSL_METHOD *method)
{
  SSL_CTX *ctx = SSL_CTX_new(method);
  if (UNLIKELY(ctx == NULL))
  {
    printf("DEB SSL_CTX_NEW ERROR\n");
    handle_ssl_error();
  }
  return ctx;
}

inline int SSL_CTX_set_min_proto_version_p(SSL_CTX *ctx, int version)
{
  if (UNLIKELY(SSL_CTX_set_min_proto_version(ctx, version) == 0))
  {
    printf("DEB SSL_CTX_SET_MIN_PROTO_VERSION ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int SSL_CTX_set_max_proto_version_p(SSL_CTX *ctx, int version)
{
  if (UNLIKELY(SSL_CTX_set_max_proto_version(ctx, version) == 0))
  {
    printf("DEB SSL_CTX_SET_MAX_PROTO_VERSION ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int SSL_CTX_set_ciphersuites_p(SSL_CTX *ctx, const char *str)
{
  if (UNLIKELY(SSL_CTX_set_ciphersuites(ctx, str) == 0))
  {
    printf("DEB SSL_CTX_SET_CIPHERSUITES ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int SSL_CTX_set_cipher_list_p(SSL_CTX *ctx, const char *str)
{
  if (UNLIKELY(SSL_CTX_set_cipher_list(ctx, str) == 0))
  {
    printf("DEB SSL_CTX_SET_CIPHER_LIST ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline SSL *SSL_new_p(SSL_CTX *ctx)
{
  SSL *ssl = SSL_new(ctx);
  if (UNLIKELY(ssl == NULL))
  {
    printf("DEB SSL_NEW ERROR\n");
    handle_ssl_error();
  }
  return ssl;
}

inline int SSL_set_fd_p(SSL *ssl, int fd)
{
  const int ret = SSL_set_fd(ssl, fd);
  if (UNLIKELY(ret == 0))
  {
    printf("DEB SSL_SET_FD ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int SSL_write_p(SSL *ssl, const void *buf, int num)
{
  const int ret = SSL_write(ssl, buf, num);
  if (LIKELY(ret <= 0))
  {
    const int ssl_error = SSL_get_error(ssl, ret);
    if (LIKELY(ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ))
      return 0;
    printf("DEB SSL_WRITE ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int SSL_read_p(SSL *ssl, void *buf, int num)
{
  const int ret = SSL_read(ssl, buf, num);
  if (LIKELY(ret <= 0))
  {
    const int ssl_error = SSL_get_error(ssl, ret);
    if (LIKELY(ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ))
      return 0;
    printf("DEB SSL_READ ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int SSL_connect_p(SSL *ssl)
{
  const int ret = SSL_connect(ssl);
  if (LIKELY(ret <= 0))
  {
    const int ssl_error = SSL_get_error(ssl, ret);
    if (LIKELY(ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ))
      return 0;
    printf("DEB SSL_connect ERROR\n");
    handle_ssl_error();
  }
  return ret;
}


static void handle_ssl_error(void)
{
  char msg[256] = {0};
  ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
  panic(msg);
}