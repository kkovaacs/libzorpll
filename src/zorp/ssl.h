/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: ssl.h,v 1.10 2004/01/20 16:58:54 bazsi Exp $
 *
 ***************************************************************************/

#ifndef ZORP_SSL_H_INCLUDED
#define ZORP_SSL_H_INCLUDED

#include <glib.h>
#include <openssl/ssl.h>
#include <zorp/zorplib.h>
#include <zorp/stream.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Class to encapsulate the data for an SSL session.
 **/
typedef struct _ZSSLSession 
{
  guint ref_cnt;
  SSL *ssl;
  gchar *session_id;
  gint verify_type;
  gint verify_depth;
  X509_STORE *crl_store;
} ZSSLSession;

#define Z_SSL_MODE_CLIENT  0
#define Z_SSL_MODE_SERVER  1

#define Z_SSL_VERIFY_NONE                0
#define Z_SSL_VERIFY_OPTIONAL            1
#define Z_SSL_VERIFY_REQUIRED_UNTRUSTED  2
#define Z_SSL_VERIFY_REQUIRED_TRUSTED    3


void z_ssl_init(void);
void z_ssl_destroy(void);

#ifndef G_OS_WIN32

#if ZORPLIB_ENABLE_SSL_ENGINE
extern gchar *crypto_engine;
#endif

ZSSLSession *
z_ssl_session_new(char *session_id, 
                  int mode,
                  gchar *key_file, 
                  gchar *cert_file, 
                  gchar *ca_dir, 
                  gchar *crl_dir, 
                  int verify_depth,
                  int verify_type);

ZSSLSession *
z_ssl_session_new_inline(char *session_id, 
                         int mode,
                         GString *key_pem, 
                         GString *cert_pem, 
                         gchar *ca_dir, 
                         gchar *crl_dir, 
                         int verify_depth,
                         int verify_type);
#else // G_OS_WIN32

ZSSLSession *
z_ssl_session_new(char *session_id, 
                  int mode,
                  X509_STORE *store, 
                  int verify_depth,
                  int verify_type);


#endif // G_OS_WIN32

ZSSLSession *z_ssl_session_new_ssl(SSL *ssl);

ZSSLSession *z_ssl_session_ref(ZSSLSession *self);
void z_ssl_session_unref(ZSSLSession *self);

gchar *z_ssl_get_error_str(gchar *buf, int buflen);

BIO *z_ssl_bio_new(ZStream *stream);

#ifdef __cplusplus
}
#endif

#endif
