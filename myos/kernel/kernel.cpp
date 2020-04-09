#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/printk.h>

/*
	Extern "C" needed to disable name mangling by C++ compiler
	for C code blocks. This prevents linking errors between C
	and C++ code.
*/
extern "C" void kernel_main(void)
{
	/* Initialize terminal interface */
	// kernel::TTY tty;

	/* Newline support is left as an exercise. */
	kernel::printk("Hello, kernel World!\n");
}
