#include <fcntl.h>

#ifdef __is_libc

extern void exit(int code);
extern int main();

void _start()
{
    int ex = main();
    exit(ex);
}

#endif
