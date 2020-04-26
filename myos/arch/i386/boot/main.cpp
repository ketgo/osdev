#include <boot/multiboot.hpp>
#include <boot/console.hpp>
#include <boot/a20.hpp>
#include <boot/gdt.hpp>
#include <boot/idt.hpp>
#include <boot/pm.hpp>

#include <arch/io.hpp>
#include <arch/exception.hpp>

/**
 * Kernel entry point.
 * 
 * The method is set noreturn attribute as the execution will
 * not return after method call.
 */
extern "C" void start_kernel(void) __attribute__((noreturn));

/**
 * Entry point for kernel boot sequence. All real and/or protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 * 
 * @param multiboot_info multiboot information provided by bootloader
 */
extern "C" void main(boot::MultibootInfo *multiboot_info)
{
    /** Disable interupts in case they are enabled */
    boot::cli();

    /* Initialize the early-boot console */
    boot::console.initialize(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    boot::console.printf("Kernel boot sequence started...\n");

    /** 
     * Enable A20 gate.
     * 
     * TODO: Test with other bootloaders besides GRUB. 
     */
    boot::console.printf("Enabling A20 gate...\n");
    if (boot::enable_a20())
    {
        boot::console.printf("A20 gate not responding.\n");
    }

    /* Setup GDT */
    boot::console.printf("Setting up GDT...\n");
    boot::gdt.setup();

    /* Setup IDT */
    boot::console.printf("Setting up IDT...\n");
    boot::idt.setup();

    /* Set system in protected mode in case not already */
    boot::console.printf("Starting protected mode...\n");
    boot::start_protected_mode();

    /* Starting kernel */
    boot::console.printf("Starting kernel...\n");
    start_kernel();
}