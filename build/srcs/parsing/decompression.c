/*================================================================================

File: decompression.c                                                           
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "parsing/decompression.h"

//https://www.zlib.net/manual.html#Stream

void gzip_decompress_to_file(uint8_t *input, const uint32_t input_len, int fd)
{
  z_stream strm = {
    .zalloc = Z_NULL,
    .zfree = Z_NULL,
    .opaque = Z_NULL,
    .next_in = input,
    .avail_in = input_len,
  };
  uint8_t out[PIPE_BUF_SIZE];
  int16_t ret;
  
  inflateInit2(&strm, 16 + MAX_WBITS);

  do
  {
    strm.next_out = out;
    strm.avail_out = PIPE_BUF_SIZE;

    ret = inflate(&strm, Z_NO_FLUSH);

    PREFETCHR(strm.next_in, L0);

    write(fd, out, PIPE_BUF_SIZE - strm.avail_out);
  } while (LIKELY(ret != Z_STREAM_END));

  inflateEnd(&strm);
}