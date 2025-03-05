/*================================================================================

File: yyjson_primitives.h                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#ifndef YYJSON_PRIMITIVES_H
# define YYJSON_PRIMITIVES_H

# include <stdint.h>
# include <yyjson.h>

# include "extensions.h"
# include "errors.h"

HOT extern inline yyjson_doc * yyjson_read_fp_p(FILE *fp, yyjson_read_flag flg, const yyjson_alc *alc, yyjson_read_err *err);

#endif