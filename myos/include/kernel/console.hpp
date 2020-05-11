#ifndef ARCH_I386_CONSOLE_HPP
#define ARCH_I386_CONSOLE_HPP

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <kernel/defs.hpp>

namespace kernel
{

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

    /**
    * Update postion of cursor to the given coordinates.
    * 
    * @param column column number of the cursor
    * @param row row number of the cursor
    */
    void update_cursor(uint16_t column, uint16_t row);

public:
    /**
     * Initialize console.
     * 
     * @param fg_color inital forground color
     * @param bg_color inital background color
     */
    void __arch initialize(VGAColor fg_color, VGAColor bg_color);

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
    * Display a formated string on consol with 
    * variable arguments
    *  
    * @param s pointer to string of char
    * @param args additional arguments
    * @returns number of char displayed
    */
    int vprintf(const char *s, va_list args);

    /**
     * Display a formated string of char on console
     * 
     * @param s pointer to string of char
     * @param ... format options
     * @returns number of char displayed
     */
    int printf(const char *s, ...);
};

/** Root console used by kernel for system logs **/
extern Console console;

} // namespace kernel

#endif /* ARCH_I386_CONSOLE_HPP */
