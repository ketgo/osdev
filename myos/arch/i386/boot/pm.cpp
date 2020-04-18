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
void boot::start_protected_mode()
{
    asm volatile(
        "movl %cr0, %eax\n\t"
        "orb $1, %eax\n\t"
        "movl %eax, %cr0\n\t");
}
