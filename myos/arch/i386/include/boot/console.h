#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>

namespace boot
{
class Console
{
public:
    void init();
    int putchar(int c);
    int printf(const char *data, ...);
}
} // namespace boot
#endif /* CONSOLE_H */
