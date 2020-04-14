#include <kernel/printk.hpp>

/*
	Extern "C" needed to disable name mangling by C++ compiler
	for C code blocks. This prevents linking errors between C
	and C++ code.
*/
extern "C" void start_kernel(void)
{
	/**
	 * Kernel start entry point.
	 */

	/* Newline support is left as an exercise. */
	kernel::printk("Hello, kernel World!\n");
}
