#include <stdarg.h>
#include <stddef.h>
#include <cstdio>

#include <kernel/tty.hpp>
#include <kernel/printf.hpp>

#define KPRINTF_BUFSIZ 64

/*
 * Log to the first console.
 */
static void printvf(const char *fmt, va_list arg)
{
    char str[KPRINTF_BUFSIZ];
    int n;

    n = std::vsnprintf(str, KPRINTF_BUFSIZ, fmt, arg);
    if (n > 0)
        kernel::tty.write(str, (size_t)n);
}

void kernel::printf(const char *__restrict fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    printvf(fmt, ap);
    va_end(ap);
}