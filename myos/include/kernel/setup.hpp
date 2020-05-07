/**
 * Architecture specific setup method invoked by the kernel.
 */

#ifndef SETUP_HPP
#define SETUP_HPP

#include <kernel/defs.hpp>

namespace kernel
{
/**
* Architecture specific setup routine invoked by the kernel. 
* Each arch should implement this method.
*/
void __arch arch_setup();

} // namespace kernel

#endif /* SETUP_HPP */
