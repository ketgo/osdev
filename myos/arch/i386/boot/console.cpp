#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <cstring>
#include <cstdio>

#include <boot/console.hpp>

#include <arch/vga.hpp>
#include <arch/io.hpp>

boot::Console boot::console;

/**
 * Update postion of cursor to the given coordinates.
 * 
 * @param column column number of the cursor
 * @param row row number of the cursor
 */
static void update_cursor(uint16_t column, uint16_t row)
{
    // get location
    uint16_t cursorLocation = row * VGA_WIDTH + column;

    boot::outb(14, 0x3D4);
    boot::outb(cursorLocation >> 8, 0x3D5); // Send the high byte.
    boot::outb(15, 0x3D4);
    boot::outb(cursorLocation, 0x3D5); // Send the low byte.
}

void boot::Console::initialize(enum VGAColor fg_color, enum VGAColor bg_color)
{
    color = uint8_t(fg_color) | uint8_t(bg_color) << 4;
    buffer = (uint16_t *)VGA_CGA_MEMORY;
    clrscr();
}

void boot::Console::set_fg_color(enum VGAColor fg_color)
{
    color = uint8_t(fg_color) | (color & 0xf0);
}

void boot::Console::set_bg_color(enum VGAColor bg_color)
{
    color = (color & 0x0f) | uint8_t(bg_color) << 4;
}

void boot::Console::clrscr()
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

int boot::Console::putchar(int c)
{
    unsigned char uc = (unsigned char)c;
    const size_t index = row * VGA_WIDTH + column;

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
    update_cursor(column, row);

    return 1;
}

int boot::Console::puts(const char *s)
{
    int written = 0;
    size_t size = std::strlen(s);

    for (size_t i = 0; i < size; i++)
    {
        if (s[i] == '\n')
        {
            column = 0;
            row++;
            continue;
        }
        putchar(s[i]);
        written++;
    }

    return written;
}

int boot::Console::printf(const char *s, ...)
{
    char buff[VGA_WIDTH];
    va_list args;

    va_start(args, s);
    std::vsnprintf(buff, VGA_WIDTH, s, args);
    va_end(args);

    return puts(buff);
}
