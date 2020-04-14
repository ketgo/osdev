#include <stdarg.h>
#include <stddef.h>
#include <cstdio>

#include <kernel/tty.hpp>
#include <kernel/printk.hpp>

#define KPRINTF_BUFSIZ 64

/*
 * Log to the first console.
 */
void printvk(const char *fmt, va_list arg)
{
    char str[KPRINTF_BUFSIZ];
    int n;

    n = std::vsnprintf(str, KPRINTF_BUFSIZ, fmt, arg);
    if (n > 0)
        kernel::tty.write(str, (size_t)n);
}

void kernel::printk(const char *__restrict fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    printvk(fmt, ap);
    va_end(ap);
}