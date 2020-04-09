#include <stdarg.h>
#include <cstdio>

#include <kernel/tty.h>
#include <kernel/printk.h>

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
        kernel::tty.write(str, n);
}

void kernel::printk(const char *__restrict fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    printvk(fmt, ap);
    va_end(ap);
}