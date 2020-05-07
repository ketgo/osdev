#ifndef PRINTF_HPP
#define PRINTF_HPP

#include <stdarg.h>

namespace kernel
{

/**
 * vprintf - print text string on system consol with variable arguments
 * 
 * This method should be used to print from inside
 * the kernel.
 * 
 * @param fmt text string to print 
 * @param args additional arguments
 */
void vprintf(const char *fmt, va_list args);

/**
 * printf - print text string on system consol
 * 
 * This method should be used to print from inside
 * the kernel.
 *
 * @param fmt text string to print
 */
void printf(const char *__restrict fmt, ...);

} // namespace kernel

#endif /* PRINTF_HPP */
