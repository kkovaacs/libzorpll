#include <zorp/stream.h>
#include <zorp/streamssl.h>
#include <zorp/streamfd.h>
#include <zorp/log.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

gchar testcert[512];
gchar testkey[512];

gint
test_server(gint fd)
{
  ZStream *stream;
  ZSSLSession *ssl_session;
  gsize bw, br;
  gchar buf[512];
  
  ssl_session = z_ssl_session_new("server/ssl", Z_SSL_MODE_SERVER, testkey, testcert, NULL, NULL, 9, Z_SSL_VERIFY_NONE);
  g_return_val_if_fail(ssl_session, 1);
  
  stream = z_stream_fd_new(fd, "server");
  stream = z_stream_push(stream, 
                         z_stream_ssl_new(NULL, ssl_session));
  
  SSL_accept(ssl_session->ssl);
  z_stream_write(stream, "helloka", 7, &bw, NULL);
  z_stream_read(stream, buf, sizeof(buf), &br, NULL);
  printf("%.*s", br, buf);
  if (memcmp(buf, "haliho", br) == 0)
    return 0;
  return 1;
}

gint 
test_client(gint fd)
{
  ZStream *stream;
  ZSSLSession *ssl_session;
  gchar buf[512];
  gsize bw, br;
  
  ssl_session = z_ssl_session_new("client/ssl", Z_SSL_MODE_CLIENT, NULL, NULL, NULL, NULL, 9, Z_SSL_VERIFY_NONE);
  g_return_val_if_fail(ssl_session, 1);
  
  stream = z_stream_fd_new(fd, "client");
  stream = z_stream_push(stream, z_stream_ssl_new(NULL, ssl_session));
  SSL_connect(ssl_session->ssl);
  
  
  z_stream_write(stream, "haliho", 6, &bw, NULL);
  z_stream_read(stream, buf, sizeof(buf), &br, NULL);
  printf("%.*s", br, buf);
  if (memcmp(buf, "helloka", br) == 0)
    return 0;
  return 1;
}

int 
main(void)
{
  gint fds[2], rc, status;
  gchar *srcdir = getenv("srcdir");
  
  z_ssl_init();
  g_snprintf(testcert, sizeof(testcert), "%s/testx509.crt", srcdir);
  g_snprintf(testkey, sizeof(testkey), "%s/testx509.key", srcdir);
  
  if (socketpair(PF_UNIX, SOCK_STREAM, 0, fds) < 0)
    {
      perror("socketpair");
      return 1;
    }
  
  if (fork() != 0)
    {
      close(fds[1]);
      return test_server(fds[0]);
    }
  else
    {
      close(fds[0]);
      rc = test_client(fds[1]);
      
      wait(&status);
      if (status)
        rc = 1;
    }
  return rc;
}
