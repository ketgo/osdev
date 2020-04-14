#ifndef KERNEL_TTY_HPP
#define KERNEL_TTY_HPP

#include <termios.h>
#include <stddef.h>
#include <stdint.h>

namespace kernel
{
#define MAX_CANON 256

/**
 * TTY class used by the kernel to interface with a console device such 
 * as a VGA screen.
 */
class TTY
{
private:
    /**
     * TODO: Add device attributes and support multiple consoles. 
     *       For now we interface with a single display console
     */

    struct termios attr;    /* termios attributes */
    char buffer[MAX_CANON]; /*<< Canonical input line */
    size_t read_pos;        /*<< Input line position to read */
    size_t write_pos;       /*<< Input line position to write */

public:
    /**
    * Initialize terminal
    */
    void init(void);

    /**
    * Write a single char on terminal
    * 
    * @param c charecter to write on terminal
    * @returns number of characters written
    */
    int putc(int c);

    /**
    * Write string of chars on terminal.
    * 
    * @param buffer pointer to memory address of the first char
    * @param n number of char to write
    * @returns number of characters written
    */
    int write(const char *buffer, size_t n);

    /**
     * Constructor to initialize tty
     */
    TTY() { this->init(); }
};

/**
 * TTY object to interface with console
 * TODO: Add class to interface with multiple tty
 */
extern TTY tty;

} // namespace kernel

#endif /* KERNEL_TTY_HPP */
