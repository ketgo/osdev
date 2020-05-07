#include <stdarg.h>
#include <stddef.h>
#include <cstdio>

#include <kernel/tty.hpp>
#include <kernel/printf.hpp>

#define KPRINTF_BUFSIZ 64

/*
 * Print to the first console.
 */
void kernel::vprintf(const char *fmt, va_list args)
{
    char str[KPRINTF_BUFSIZ];
    int n;

    n = std::vsnprintf(str, KPRINTF_BUFSIZ, fmt, args);
    if (n > 0)
        kernel::tty.write(str, (size_t)n);
}

void kernel::printf(const char *__restrict fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}