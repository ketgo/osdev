#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <kernel/ioport.hpp>
#include <kernel/console.hpp>

/**
 * VGA real mode memory location and attributes. 
 * Remember: We are in Mode 7, i.e. 80 columns by 
 * 25 lines of high resolution text characters.
 */

/** EGA mode memory map start address */
#define VGA_EGA_MEMORY 0xA0000

/** MDA mode memory map start address */
#define VGA_MDA_MEMORY 0xB0000

/** CGA mode memory map start address */
#define VGA_CGA_MEMORY 0xB8000

void kernel::Console::update_cursor(uint16_t column, uint16_t row)
{
    // get location
    uint16_t cursorLocation = row * VGA_WIDTH + column;

    kernel::outb(14, 0x3D4);
    kernel::outb(cursorLocation >> 8, 0x3D5); // Send the high byte.
    kernel::outb(15, 0x3D4);
    kernel::outb(cursorLocation, 0x3D5); // Send the low byte.
}

void kernel::Console::initialize(enum VGAColor fg_color, enum VGAColor bg_color)
{
    color = uint8_t(fg_color) | uint8_t(bg_color) << 4;
    buffer = (uint16_t *)VGA_CGA_MEMORY;
    set_fg_color(fg_color);
    set_bg_color(bg_color);
    clrscr();
}
