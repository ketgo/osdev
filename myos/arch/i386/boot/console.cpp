#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <cstring>
#include <cstdio>

#include <boot/vga.hpp>
#include <boot/console.hpp>

boot::Console boot::console;

void boot::Console::initialize(enum VGAColor fg_color, enum VGAColor bg_color)
{
    this->row = 0;
    this->column = 0;
    this->color = fg_color | bg_color << 4;
    this->buffer = (uint16_t *)VGA_CGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            this->buffer[index] = ' ' | (uint16_t)this->color << 8;
        }
    }
}

void boot::Console::set_fg_color(enum VGAColor color)
{
    this->color = color | this->color << 4;
}

void boot::Console::set_bg_color(enum VGAColor color)
{
    this->color = this->color | color << 4;
}

int boot::Console::putchar(int c)
{
    unsigned char uc = (unsigned char)c;
    const size_t index = this->row * VGA_WIDTH + this->column;

    this->buffer[index] = uc | (uint16_t)this->color << 8;
    if (++this->column == VGA_WIDTH)
    {
        this->column = 0;
        if (++this->row == VGA_HEIGHT)
        {
            this->row = 0;
        }
        else
        {
            this->row++;
        }
    }

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
            this->column = 0;
            this->row++;
            continue;
        }
        this->putchar(s[i]);
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

    return this->puts(buff);
}
