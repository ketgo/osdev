/**
 * Kernel entry point
 */
extern "C" void start_kernel(void);

/**
 * Entry point for kernel boot sequence. All real and protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 */
extern "C" void main(void) {
    start_kernel();
}