/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/12 12:36:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_H
# define SSL_H

# include <stdint.h>
# include <wolfssl/options.h>
# include <wolfssl/ssl.h>
# include <wolfssl/wolfcrypt/ed25519.h>
# include <wolfssl/wolfcrypt/base64.h>

# include "keys.h"

typedef struct
{
  WOLFSSL_CTX *ctx;
  WOLFSSL *ssl;
} ssl_sock_t;

typedef struct
{
  WC_RNG rng;
} ssl_data_t;

void  init_ssl(void);
void  init_ssl_socket(const uint16_t fd, ssl_sock_t *const ssl);
//TODO void  generate_signature(
void  cleanup_ssl_socket(ssl_sock_t *const ssl);

#endif