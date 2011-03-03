AM_CPPFLAGS=-I$(top_srcdir)/src -I../src -Wno-error=format -Wno-error=int-to-pointer-cast -Wno-error=pointer-sign -Wno-error=shadow -Wno-error=sign-compare -Wno-error=strict-prototypes -Wno-error=unused-result -Wno-error=unused-variable

check_PROGRAMS = zcrypt test_readline test_registry test_conns test_ssl test_streams test_random test_valid_chars test_sockaddr test_blob test_base64 test_codegzip test_codecipher portrandom

zcrypt_SOURCES = zcrypt.c
zcrypt_LDADD = ../src/libzorpll.la

test_readline_SOURCES = test_readline.c
test_readline_LDADD = ../src/libzorpll.la

test_registry_SOURCES = test_registry.c
test_registry_LDADD = ../src/libzorpll.la

test_conns_SOURCES = test_conns.c
test_conns_LDADD = ../src/libzorpll.la

test_ssl_SOURCES = test_ssl.c
test_ssl_LDADD = ../src/libzorpll.la

test_random_SOURCES = test_random.c
test_random_LDADD = ../src/libzorpll.la

test_streams_SOURCES = test_streams.c
test_streams_LDADD = ../src/libzorpll.la

test_valid_chars_SOURCES = test_valid_chars.c
test_valid_chars_LDADD = ../src/libzorpll.la

test_sockaddr_SOURCES = test_sockaddr.c
test_sockaddr_LDADD = ../src/libzorpll.la

test_blob_SOURCES = test_blob.c
test_blob_LDADD = ../src/libzorpll.la

test_base64_SOURCES = test_base64.c
test_base64_LDADD = ../src/libzorpll.la

test_codegzip_SOURCES = test_codegzip.c
test_codegzip_LDADD = ../src/libzorpll.la

test_codecipher_SOURCES = test_codecipher.c
test_codecipher_LDADD = ../src/libzorpll.la

portrandom_SOURCES = portrandom.c randtest.c randtest.h
portrandom_LDADD = ../src/libzorpll.la -lm

TESTS = test_registry test_readline zcrypt test_conns test_ssl test_random test_streams test_valid_chars test_sockaddr test_blob test_base64 test_codegzip test_codecipher portrandom
