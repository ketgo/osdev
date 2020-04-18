#ifndef ARCH_I386_A20_HPP
#define ARCH_I386_A20_HPP

namespace boot
{
/**
 * Enable A20 line on x86 processor.
 * 
 * @returns 0 on success and -1 on failure
 */
int enable_a20();

} // namespace boot

#endif /* ARCH_I386_A20_HPP */
