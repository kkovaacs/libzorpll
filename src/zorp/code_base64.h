#ifndef ZORP_PROXY_CODE_BASE64_H
#define ZORP_PROXY_CODE_BASE64_H

#include <zorp/code.h>

ZCode *z_code_base64_encode_new(gint bufsize, gint linelen);
ZCode *z_code_base64_decode_new(gint bufsize, gboolean error_tolerant);

#endif
