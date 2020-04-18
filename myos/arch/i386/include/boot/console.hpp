#ifndef ARCH_I386_CONSOLE_HPP
#define ARCH_I386_CONSOLE_HPP

#include <stddef.h>
#include <stdint.h>
#include <boot/vga.hpp>

namespace boot
{
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

} // namespace boot

#endif /* ARCH_I386_CONSOLE_HPP */
