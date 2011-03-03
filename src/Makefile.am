EXTRA_DIST = makefile.msc libzorpll.rc libzorpll.def

SUBDIRS = zorp

lib_LTLIBRARIES = libzorpll.la

libzorpll_la_SOURCES = \
	cap.c \
	connect.c \
	crypt.c \
	error.c \
	io.c \
	listen.c \
	log.c \
	memtrace.c \
	misc.c \
	packetbuf.c \
	poll.c \
	process.c \
	random.c \
	registry.c \
	sockaddr.c \
	socket.c \
	socketsource.c \
	source.c \
	ssl.c \
	stackdump.c \
	stream.c \
	streambuf.c \
	streamfd.c \
	streamgzip.c \
	streamline.c \
	streamssl.c \
	thread.c \
	zobject.c \
	blob.c \
	streamblob.c \
	streamtee.c \
	code_base64.c \
	code.c \
	code_cipher.c \
	code_gzip.c \
	zurlparse.c
	

libzorpll_la_LDFLAGS = \
	-version-info $(LIBZORPLL_LT_CURRENT):0:$(LIBZORPLL_LT_AGE) \
	-release $(LIBZORPLL_LT_RELEASE) \
	-export-dynamic

libzorpll_la_CPPFLAGS = -DCOMPILING_LIBZORPLL

install-exec-local:
	mkdir -p ${DESTDIR}/${ZORPLIB_TEMP_DIR}
	mkdir -p ${DESTDIR}/${ZORPLIB_PIDFILE_DIR}
