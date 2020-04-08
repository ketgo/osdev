#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stddef.h>

/**
 * Terminal Interface
 * ==================
 * 
 * Functions to interface with terminal used by the kernel library
 * for display. The implementation of these declared functions is 
 * architecture specific.
 */
namespace terminal
{
    /**
     * Initialize terminal
     */
void initialize(void);

/**
 * Write a single char on terminal
 * 
 * @param c charecter to write on terminal
 */
void putc(char c);

/**
 * Write collection of char at an address on terminal.
 * 
 * @param data pointer to memory address of first char
 * @param size number of char to write on terminal
 */
void writevp(const char *data, size_t size);

/**
 * Write string of chars on terminal.
 * 
 * @param data pointer to memory address of the first char
 */
void puts(const char *data);
} // namespace terminal

#endif
