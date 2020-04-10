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

    terminal_initialize();

    terminal_putsv("Initial bootstrap started...\n");

    start_kernel();
}