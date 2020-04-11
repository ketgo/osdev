#include <boot/console.h>


/**
 * Kernel entry point
 */
extern "C" void start_kernel(void);

/**
 * Entry point for kernel boot sequence. All real and protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 */
extern "C" void main(void) {

    boot::console.initialize(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    boot::console.printf("Initial bootstrap started...\nSetting up GDT...");

    start_kernel();
}