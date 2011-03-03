/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: streamssl.h,v 1.4 2003/04/08 13:32:29 sasa Exp $
 *
 ***************************************************************************/

#ifndef ZORP_STREAMSSL_H_INCLUDED
#define ZORP_STREAMSSL_H_INCLUDED

#include <zorp/ssl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ZST_CTRL_SSL_SET_SESSION     (0x01) | ZST_CTRL_SSL_OFS

ZStream * z_stream_ssl_new(ZStream *stream, ZSSLSession *ssl);

static inline void
z_stream_ssl_set_session(ZStream *self, ZSSLSession *ssl)
{
  z_stream_ctrl(self, ZST_CTRL_SSL_SET_SESSION, ssl, sizeof(&ssl));
}

#ifdef __cplusplus
}
#endif

#endif /* ZORP_GIOSSL_H_INCLUDED */
