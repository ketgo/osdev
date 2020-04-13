#include <boot/console.h>
#include <boot/gdt.h>


/**
 * Kernel entry point
 */
extern "C" void start_kernel(void);

/**
 * Entry point for kernel boot sequence. All real and protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 */
extern "C" void main(void) {

    boot::console.initialize(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    boot::console.printf("Kernel boot sequence started...\n");
    
    boot::console.printf("Setting up GDT...\n");
    boot::gdt.initialize();

    start_kernel();
}