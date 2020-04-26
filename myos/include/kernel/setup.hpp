/**
 * Arch specific setup method invoked by the kernel.
 */

#ifndef SETUP_HPP
#define SETUP_HPP

namespace arch
{
/**
* Architecture specific setup routine invoked by the kernel. 
* Each arch should implement this method.
*/
extern void setup();

} // namespace kernel

#endif /* SETUP_HPP */
