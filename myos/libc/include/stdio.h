#ifndef STDIO_H
#define STDIO_H 1

#include <stddef.h>
#include <stdarg.h>

#define EOF (-1)

/**
* Write formatted data from variable argument list to sized buffer
* 
* Composes a string with the same text that would be printed if format was used on printf, 
* but using the elements in the variable argument list identified by arg instead of additional 
* function arguments and storing the resulting content as a C string in the buffer pointed by 
* s (taking n as the maximum buffer capacity to fill).
* 
* If the resulting string would be longer than n-1 characters, the remaining characters are 
* discarded and not stored, but counted for the value returned by the function.
* 
* Internally, the function retrieves arguments from the list identified by arg as if va_arg was 
* used on it, and thus the state of arg is likely to be altered by the call.
* 
* In any case, arg should have been initialized by va_start at some point before the call, and it 
* is expected to be released by va_end at some point after the call.
* 
* @param s Pointer to a buffer where the resulting C-string is stored. The buffer should have a 
*   size of at least n characters.
* @param n Maximum number of bytes to be used in the buffer. The generated string has a length of 
*   at most n-1, leaving space for the additional terminating null character. size_t is an unsigned 
*   integral type.
* @param format C string that contains a format string that follows the same specifications as format 
*   in printf (see printf for details).
* @param arg A value identifying a variable arguments list initialized with va_start. va_list is a 
*   special type defined in <stdarg.h>. 
* @returns number of char written to sized buffer
*/
int vsnprintf(char *s, size_t n, const char *format, va_list arg);

/**
 * Write formatted output to sized buffer
 * 
 * Composes a string with the same text that would be printed if format was used on printf, but 
 * instead of being printed, the content is stored as a C string in the buffer pointed by s (taking 
 * n as the maximum buffer capacity to fill).
 * 
 * If the resulting string would be longer than n-1 characters, the remaining characters are discarded 
 * and not stored, but counted for the value returned by the function.
 * 
 * A terminating null character is automatically appended after the content written.
 * 
 * After the format parameter, the function expects at least as many additional arguments as needed 
 * for format.
 * 
 * @param s Pointer to a buffer where the resulting C-string is stored. The buffer should have a 
 *   size of at least n characters.
 * @param n Maximum number of bytes to be used in the buffer. The generated string has a length of 
 *   at most n-1, leaving space for the additional terminating null character. size_t is an unsigned 
 *   integral type.
 * @param format C string that contains a format string.
 * @returns number of char written to sized buffer
 */
int snprintf(char *s, size_t n, const char *format, ...);

#endif
