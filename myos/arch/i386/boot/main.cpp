#include <boot/multiboot.hpp>

#include <kernel/console.hpp>
#include <kernel/ioport.hpp>

#include <i386/a20.hpp>
#include <i386/gdt.hpp>
#include <i386/idt.hpp>
#include <i386/pm.hpp>

using namespace kernel;

/**
 * Kernel entry point.
 * 
 * The method is set noreturn attribute as the execution will
 * not return after method call.
 */
extern "C" void start_kernel(boot::MultibootInfo *multiboot_info) __attribute__((noreturn));

/**
 * Entry point for kernel boot sequence. All real and/or protected mode setup 
 * required for the correct functioning of the kernel is pereformed here.
 * 
 * @param multiboot_info multiboot information provided by bootloader
 */
extern "C" void main(boot::MultibootInfo *multiboot_info)
{
    /* Initialize the system console */
    console.initialize(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    console.printf("Kernel boot sequence started...\n");

    /** 
     * Enable A20 gate.
     * 
     * TODO: Test with other bootloaders besides GRUB. 
     */
    console.printf("Enabling A20 gate...\n");
    if (I386::enable_a20())
    {
        console.printf("A20 gate not responding.\n");
        hang();
    }

    /* Setup GDT */
    console.printf("Setting up GDT...\n");
    I386::GDT::setup();

    /* Setup IDT */
    console.printf("Setting up IDT...\n");
    I386::IDT::setup();

    /* Set system in protected mode in case not already */
    console.printf("Starting protected mode...\n");
    I386::start_protected_mode();

    /* Starting kernel */
    console.printf("Starting kernel...\n");
    start_kernel(multiboot_info);
}