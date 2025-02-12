/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_serializer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 11:15:12 by craimond          #+#    #+#             */
/*   Updated: 2025/02/09 09:34:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERIALIZER_H
# define HTTP_SERIALIZER_H

# include <stdint.h>
# include <string.h>
# include <stdlib.h>
# include <xxhash.h>
# include <immintrin.h>

# include "primitives/alloc_primitives.h"
# include "extensions.h"
# include "logger.h"
# include "errors.h"
# include "string_utils.h"

# define HEADER_MAP_DILUTION_FACTOR 5

typedef enum: uint8_t {GET, POST, PUT, DELETE} http_method_t;
typedef enum: uint8_t {HTTP_1_0, HTTP_1_1} http_version_t;

typedef struct ALIGNED(16)
{
  char *key;
  uint8_t key_len;
  char *value;
  uint8_t value_len;
} header_entry_t;

typedef struct
{
  header_entry_t *entries;
  uint8_t n_entries;
  uint16_t size;
} header_map_t;

typedef struct ALIGNED(16)
{
  const http_method_t method;
  const char *path;
  const uint8_t path_len;
  const http_version_t version;
  const header_entry_t *headers;
  const uint8_t n_headers;
  const char *body;
  const uint16_t body_len;
} http_request_t;

typedef struct ALIGNED(16)
{
  uint16_t status_code;
  http_version_t version;
  header_map_t headers;
  char *body;
  uint16_t body_len;
} http_response_t;

HOT uint32_t serialize_http_request(char *restrict buffer, const uint32_t buffer_size, const http_request_t *restrict request);
HOT bool is_full_http_response(const char *restrict buffer, const uint32_t buffer_size, const uint32_t response_len);
HOT uint32_t deserialize_http_response(const char *restrict buffer, const uint32_t buffer_size, http_response_t *restrict response);
HOT const header_entry_t *header_map_get(const header_map_t *restrict map, const char *restrict key, const uint16_t key_len);
HOT void free_http_response(http_response_t *response);

#endif