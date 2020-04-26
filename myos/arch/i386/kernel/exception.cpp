#include <stdint.h>

#include <boot/console.hpp>

#include <arch/isr.hpp>
#include <arch/exception.hpp>

//! divide by 0 fault
void boot::divide_by_zero_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] Error Code: 0x%x", state->cs, state->eip, state->eflags, state->err_code);
    for (;;)
        ;
}

//! single step
void boot::single_step_trap(boot::ISRFrame *const state)
{

    boot::console.printf("Single step");
    for (;;)
        ;
}

//! non maskable interrupt trap
void boot::nmi_trap(boot::ISRFrame *const state)
{

    boot::console.printf("NMI trap");
    for (;;)
        ;
}

//! breakpoint hit
void boot::breakpoint_trap(boot::ISRFrame *const state)
{

    boot::console.printf("Breakpoint trap");
    for (;;)
        ;
}

//! overflow
void boot::overflow_trap(boot::ISRFrame *const state)
{

    boot::console.printf("Overflow trap");
    for (;;)
        ;
}

//! bounds check
void boot::bounds_check_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Bounds check fault");
    for (;;)
        ;
}

//! invalid opcode / instruction
void boot::invalid_opcode_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Invalid opcode");
    for (;;)
        ;
}

//! device not available
void boot::no_device_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Device not found");
    for (;;)
        ;
}

//! double fault
void boot::double_fault_abort(boot::ISRFrame *const state)
{

    boot::console.printf("Double fault");
    for (;;)
        ;
}

//! invalid Task State Segment (TSS)
void boot::invalid_tss_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Invalid TSS");
    for (;;)
        ;
}

//! segment not present
void boot::no_segment_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Invalid segment");
    for (;;)
        ;
}

//! stack fault
void boot::stack_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Stack fault");
    for (;;)
        ;
}

//! general protection fault
void boot::general_protection_fault(boot::ISRFrame *const state)
{

    boot::console.printf("General Protection Fault");
    for (;;)
        ;
}

//! page fault
void boot::page_fault(boot::ISRFrame *const state)
{
    uint32_t addr;

    asm volatile("mov %%cr2, %0"
                 : "=a"(addr)
                 :
                 :);
    boot::console.printf("Page Fault at 0x%x:0x%x referenced memory at 0x%x", state->cs, state->eip, addr);
    for (;;)
        ;
}

//! Floating Point Unit (FPU) error
void boot::fpu_fault(boot::ISRFrame *const state)
{

    boot::console.printf("FPU Fault");
    for (;;)
        ;
}

//! alignment check
void boot::alignment_check_fault(boot::ISRFrame *const state)
{

    boot::console.printf("Alignment Check");
    for (;;)
        ;
}

//! machine check
void boot::machine_check_abort(boot::ISRFrame *const state)
{

    boot::console.printf("Machine Check");
    for (;;)
        ;
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void boot::simd_fpu_fault(boot::ISRFrame *const state)
{

    boot::console.printf("FPU SIMD fault");
    for (;;)
        ;
}
