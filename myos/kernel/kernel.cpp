#include <boot/multiboot.hpp>

#include <kernel/setup.hpp>
#include <kernel/printf.hpp>

using namespace kernel;

/**
 * Kernel start entry point.
 * 
 * @param multiboot_info multiboot information
 */
extern "C" void start_kernel(boot::MultibootInfo *multiboot_info)
{
	// Setup arch
	arch_setup();

	printf("Hello, kernel World!\n");

	for (;;)
		;
}
