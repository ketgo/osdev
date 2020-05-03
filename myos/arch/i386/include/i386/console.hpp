#ifndef ARCH_I386_CONSOLE_HPP
#define ARCH_I386_CONSOLE_HPP

#include <stddef.h>
#include <stdint.h>

namespace I386
{

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

/** VGA text mode width and height **/
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/** VGA 16 bit color code */
enum VGAColor
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
};

class Console
{
private:
    size_t row;
    size_t column;
    uint8_t color;
    uint16_t *buffer;

public:
    /**
     * Initialize console.
     * 
     * @param fg_color inital forground color
     * @param bg_color inital background color
     */
    void initialize(VGAColor fg_color, VGAColor bg_color);

    /**
     * Set forground color
     * 
     * @param color colort to set
     */
    void set_fg_color(VGAColor color);

    /**
     * Set background color
     * 
     * @param color colort to set
     */
    void set_bg_color(VGAColor color);

    /**
     * Clear screen
     */
    void clrscr();

    /**
     * Goto postion (column, row) on screen
     * 
     * @param column column number to goto on screen
     * @param row row number to goto on screen
     */
    void gotoxy(size_t column, size_t row);

    /**
     * Display a char on console
     * 
     * @param c char to display
     * @returns number of char displayed
     */
    int putchar(int c);

    /**
     * Display a string of char on console
     * 
     * @param s pointer to string of char
     * @returns number of char displayed
     */
    int puts(const char *s);

    /**
     * Display a formated string of char on console
     * 
     * @param s pointer to string of char
     * @param ... format options
     * @returns number of char displayed
     */
    int printf(const char *s, ...);
};

/** console used by kernel boot sequence **/
extern Console console;

} // namespace I386

#endif /* ARCH_I386_CONSOLE_HPP */
