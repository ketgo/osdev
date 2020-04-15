#include <boot/multiboot.hpp>
#include <boot/console.hpp>
#include <boot/pm.hpp>

/**
 * Kernel entry point
 */
extern "C" void start_kernel(void);

/**
 * Entry point for kernel boot sequence. All real and/or protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 * 
 * @param multiboot_info multiboot information provided by bootloader
 */
extern "C" void main(boot::MultibootInfo *multiboot_info)
{
    /* Initialize the early-boot console */
    boot::console.initialize(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    boot::console.printf("Kernel boot sequence started...\n");

    boot::console.printf("Multiboot info flags: %d\n", multiboot_info->flags);

    /* Start protected mode in case not already */
    boot::console.printf("Starting protected mode...\n");
    boot::start_protected_mode();

    start_kernel();
}