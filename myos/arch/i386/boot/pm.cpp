#include <boot/console.hpp>
#include <boot/gdt.hpp>
#include <boot/io.hpp>
#include <boot/pm.hpp>

/**
 * The method sets protected mode through the CR0 register.
 * 
 *   31        29              18        16              5                        0
 * +----+----+----+----//----+----+----+----+----//----+----+----+----+----+----+----+
 * | PG | CD | NW |          | AM |    | WP |          | NE | ET | TS | EM | ME | PE |
 * +----+----+----+----//----+----+----+----+----//----+----+----+----+----+----+----+
 * 
 * PE: If 1, system is in protected mode, else system is in real mode
 * ME: Controls interaction of WAIT/FWAIT instructions with TS flag in CR0
 * EM: If set, no x87 floating-point unit present, if clear, x87 FPU present 
 * TS: Allows saving x87 task context upon a task switch only after x87 instruction used 
 * ET: On the 386, it allowed to specify whether the external math coprocessor was an 80287 or 80387
 * NE: Enable internal x87 floating point error reporting when set, else enables PC style x87 error detection 
 * WP: When set, the CPU can't write to read-only pages when privilege level is 0 
 * AM: Alignment check enabled if AM set, AC flag (in EFLAGS register) set, and privilege level is 3 
 * NW: Globally enables/disable write-through caching
 * CD: Globally enables/disable the memory cache 
 * PG: If 1, enable paging and use the § CR3 register, else disable paging.
 * 
 * The important bit here is bit 0.
 */
static void set_protected_mode()
{
    asm volatile(
        "movl %eax, %cr0\n\t"
        "orb $1, %eax\n\t"
        "movl %cr0, %eax\n\t");
}

void boot::start_protected_mode()
{
    /** 
     * Disabling inturupts if not already. Interupts in protected mode 
     * will cause the processor to triple fault as IDT are still not set.
     */
    boot::cli();

    /* Setup GDT */
    boot::console.printf("Setting up GDT...\n");
    boot::gdt.initialize();

    /* Set the system in protected mode */
    boot::console.printf("Setting protected mode...\n");
    set_protected_mode();
}