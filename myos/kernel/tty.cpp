#include <termios.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

kernel::TTY kernel::tty;

static void tty_attr_init(struct termios *termptr)
{
    termptr->c_iflag = BRKINT | ICRNL;
    termptr->c_oflag = 0;
    termptr->c_cflag = 0;
    termptr->c_lflag = ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG;
    termptr->c_cc[VEOF] = 0x04;   /* ASCII EOT */
    termptr->c_cc[VEOL] = 0x00;   /* undefined */
    termptr->c_cc[VERASE] = 0x08; /* ASCII BS */
    termptr->c_cc[VINTR] = 0x03;  /* ASCII ETX */
    termptr->c_cc[VKILL] = 0x00;  /* undefined */
    termptr->c_cc[VMIN] = 0x00;   /* undefined */
    termptr->c_cc[VQUIT] = 0x1C;  /* ASCII FS */
    termptr->c_cc[VSTART] = 0x00; /* undefined */
    termptr->c_cc[VSUSP] = 0x00;  /* undefined */
    termptr->c_cc[VTIME] = 0x00;
}

void kernel::TTY::init()
{
    this->buffer[0] = 0;
    this->read_pos = 0;
    this->write_pos = 0;

    tty_attr_init(&this->attr);
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
        if (this->putc(((const unsigned char *)buffer)[i]) < 0)
            break;
    }

    return i;
}
