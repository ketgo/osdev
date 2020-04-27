#include <termios.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>

kernel::TTY kernel::tty;

static void attr_init(struct termios *attr)
{
    attr->c_iflag = BRKINT | ICRNL;
    attr->c_oflag = 0;
    attr->c_cflag = 0;
    attr->c_lflag = ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG;
    attr->c_cc[VEOF] = 0x04;   /* ASCII EOT */
    attr->c_cc[VEOL] = 0x00;   /* undefined */
    attr->c_cc[VERASE] = 0x08; /* ASCII BS */
    attr->c_cc[VINTR] = 0x03;  /* ASCII ETX */
    attr->c_cc[VKILL] = 0x00;  /* undefined */
    attr->c_cc[VMIN] = 0x00;   /* undefined */
    attr->c_cc[VQUIT] = 0x1C;  /* ASCII FS */
    attr->c_cc[VSTART] = 0x00; /* undefined */
    attr->c_cc[VSUSP] = 0x00;  /* undefined */
    attr->c_cc[VTIME] = 0x00;
}

void kernel::TTY::init()
{
    buffer[0] = 0;
    read_pos = 0;
    write_pos = 0;

    attr_init(&attr);
}

int kernel::TTY::putc(int c)
{
    /** TODO: Interface with console */
    return 0;
}

int kernel::TTY::write(const char *buffer, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
    {
        if (putc(((const unsigned char *)buffer)[i]) < 0)
            break;
    }

    return i;
}
