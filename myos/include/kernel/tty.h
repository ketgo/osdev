#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <termios.h>

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
    unsigned int read_pos;  /*<< Input line position to read */
    unsigned int write_pos; /*<< Input line position to write */

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
    int write(const char *buffer, unsigned int n);

    /**
     * Constructor to initialize tty
     */
    TTY() { this->init(); }
}; 

static TTY tty;

} // namespace kernel

#endif
