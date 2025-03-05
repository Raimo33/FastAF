/*================================================================================

File: crypto_primitives.h                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef CRYPTO_PRIMITIVES_H
# define CRYPTO_PRIMITIVES_H

# include <stdint.h>
# include <openssl/err.h>
# include <openssl/rand.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline EVP_PKEY *EVP_PKEY_new_raw_private_key_p(int type, ENGINE *e, const uint8_t *key, size_t keylen);
HOT extern inline int32_t EVP_DigestSignInit_p(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e, EVP_PKEY *pkey);
HOT extern inline int32_t EVP_DigestSign_p(EVP_MD_CTX *ctx, uint8_t *sigret, size_t *siglen, const uint8_t *tbs, size_t tbslen);
HOT extern inline int32_t EVP_EncodeBlock_p(uint8_t *t, const uint8_t *f, int n);
HOT extern inline int32_t EVP_DecodeBlock_p(uint8_t *t, const uint8_t *f, int n);
HOT extern inline int32_t RAND_bytes_p(uint8_t *buf, int num);

#endif