AC_INIT([libc], [1.0])
AC_PREREQ(2.64)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

dnl Check for C compiler
CFLAGS="${CFLAGS} -std=gnu11 -nostdlib -nostartfiles -nodefaultlibs -ffreestanding"
LT_INIT
AC_PROG_CXX

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
