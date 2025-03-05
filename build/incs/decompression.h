/*================================================================================

File: decompression.h                                                           
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef DECOMPRESSION_H
# define DECOMPRESSION_H

# include <zlib.h>
# include <stdint.h>

# include <unistd.h>
# include <fcntl.h>

# include "extensions.h"

# define PIPE_BUF_SIZE _PC_PIPE_BUF

HOT void gzip_decompress_to_file(uint8_t *input, const uint32_t input_len, int fd);

#endif