/**
 * Routine to enable A20 gate on x86 processor.
 */

#ifndef ARCH_I386_A20_HPP
#define ARCH_I386_A20_HPP

namespace I386
{
/**
 * Enable A20 line on x86 processor.
 * 
 * @returns 0 on success and -1 on failure
 */
int enable_a20();

} // namespace I386

#endif /* ARCH_I386_A20_HPP */
