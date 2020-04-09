#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>

#include <cstdio>
#include <cstring>

int std::vsnprintf(char *s, size_t n, const char *__restrict format, va_list arg)
{
    int written = 0;

    /**
     * Just copies the string.
     * TODO: Complete implementation
    */
    while ((*format != '\0') && (written < (int)n))
    {
        s[written] = *format;
        format++;
    }

    return written;
}

int std::snprintf(char *s, size_t n, const char *__restrict format, ...)
{
    va_list args;
    int written = 0;

    va_start(args, format);
    written = std::vsnprintf(s, n, format, args);
    va_end(args);

    return written;
}
