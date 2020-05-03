#include <boot/multiboot.hpp>

#include <arch/setup.hpp>

#include <kernel/printf.hpp>

/**
 * Kernel start entry point.
 * 
 * @param multiboot_info multiboot information
 */
extern "C" void start_kernel(boot::MultibootInfo *multiboot_info)
{
	// Setup arch
	arch::setup();

	kernel::printf("Hello, kernel World!\n");

	for (;;)
		;
}
