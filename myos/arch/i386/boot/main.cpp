#include <boot/console.h>
#include <boot/pm.h>

/**
 * Kernel entry point
 */
extern "C" void start_kernel(void);

/**
 * Entry point for kernel boot sequence. All real and protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 */
extern "C" void main(void)
{
    /* Initialize the early-boot console */
    boot::console.initialize(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    boot::console.printf("Kernel boot sequence started...\n");

    /* Start protected mode */
    boot::console.printf("Starting protected mode...\n");
    boot::start_protected_mode();

    start_kernel();
}