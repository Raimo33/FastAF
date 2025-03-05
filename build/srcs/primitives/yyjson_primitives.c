/*================================================================================

File: yyjson_primitives.c                                                       
Creator: Claudio Raimondi                                                       
Email: claudio.raimondi@pm.me                                                   

created at: 2025-03-05 19:50:27                                                 
last edited: 2025-03-05 19:50:27                                                

================================================================================*/

#include "primitives/yyjson_primitives.h"

# include <stdio.h> //TODO remove

inline yyjson_doc * yyjson_read_fp_p(FILE *fp, yyjson_read_flag flg, const yyjson_alc *alc, yyjson_read_err *err)
{
  yyjson_doc *doc = yyjson_read_fp(fp, flg, alc, err);
  if (doc == NULL)
  {
    printf("DEB YYJSON_READ_FD ERROR\n");
    panic(err->msg);
  }
  return doc;
}