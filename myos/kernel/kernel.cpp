#include <boot/multiboot.hpp>

#include <kernel/setup.hpp>
#include <kernel/printf.hpp>

#include <i386/pit.hpp>

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

	uint32_t last_tick = I386::PIT::get_ticks();

	while (true)
	{
		if (I386::PIT::get_ticks() - last_tick > 100)
		{
			last_tick = I386::PIT::get_ticks();
			printf("[KERNEL] ticks = %d\n", last_tick);
		}
	}
}
