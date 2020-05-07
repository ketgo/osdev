#ifndef PANIC_HPP
#define PANIC_HPP

namespace kernel
{

/**
 * panic - halt the system
 *
 * Display a message, then perform cleanups.
 *
 * @param fmt The text string to print
 * 
 * This function never returns.
 */
void panic(const char *__restrict fmt, ...);

} // namespace kernel

#endif /* PANIC_HPP */
