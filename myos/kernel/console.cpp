#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <kernel/ioport.hpp>
#include <kernel/console.hpp>

kernel::Console kernel::console;

void kernel::Console::set_fg_color(enum VGAColor fg_color)
{
    color = uint8_t(fg_color) | (color & 0xf0);
}

void kernel::Console::set_bg_color(enum VGAColor bg_color)
{
    color = (color & 0x0f) | uint8_t(bg_color) << 4;
}

void kernel::Console::clrscr()
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = ' ' | (uint16_t)color << 8;
        }
    }
    row = 0;
    column = 0;
    update_cursor(column, row);
}

int kernel::Console::putchar(int c)
{
    unsigned char uc = (unsigned char)c;
    const size_t index = row * VGA_WIDTH + column;

    // Check for new line
    if (uc == '\n')
    {
        column = 0;
        row++;
    }
    else
    {

        buffer[index] = uc | (uint16_t)color << 8;
        if (++column == VGA_WIDTH)
        {
            column = 0;
            if (++row == VGA_HEIGHT)
            {
                row = 0;
            }
            else
            {
                row++;
            }
        }
    }
    update_cursor(column, row);

    return 1;
}

int kernel::Console::vprintf(const char *s, va_list args)
{
    int written = 0;
    size_t size = strlen(s);
    char buff[size+1];

    vsnprintf(buff, size+1, s, args);

    for (size_t i = 0; i < size; i++)
    {
        putchar(buff[i]);
        written++;
    }

    return written;
}

int kernel::Console::printf(const char *s, ...)
{
    int written;
    va_list args;

    va_start(args, s);
    written = vprintf(s, args);
    va_end(args);

    return written;
}
