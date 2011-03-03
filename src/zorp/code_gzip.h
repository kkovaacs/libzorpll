#ifndef ZORP_PROXY_CODE_GZIP_H
#define ZORP_PROXY_CODE_GZIP_H

#include <zorp/code.h>

ZCode *z_code_gzip_encode_new(gint bufsize, gint compress_level);
ZCode *z_code_gzip_decode_new(gint bufsize);

#endif
