/**
 * Architecture specific setup method invoked by the kernel.
 */

#ifndef SETUP_HPP
#define SETUP_HPP

#include <kernel/defs.hpp>

namespace arch
{
/**
* Architecture specific setup routine invoked by the kernel. 
* Each arch should implement this method.
*/
void __arch setup();

} // namespace arch

#endif /* SETUP_HPP */
