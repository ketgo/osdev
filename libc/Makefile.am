lib_LIBRARIES = libk.a
include_HEADERS = \
${srcdir}/include/string.h \
${srcdir}/include/stdio.h \
${srcdir}/include/termios.h \
${srcdir}/include/time.h \
${srcdir}/include/cstdio \
${srcdir}/include/cstring

libk_a_CFLAGS = -I$(srcdir)/include

libk_a_SOURCES = \
${srcdir}/stdio.c \
${srcdir}/string.c

all: libk.a