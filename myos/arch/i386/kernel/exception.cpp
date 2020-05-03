#include <stdint.h>

#include <i386/console.hpp>
#include <i386/isr.hpp>
#include <i386/exception.hpp>

//! divide by 0 fault
void I386::divide_by_zero_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] Error Code: 0x%x", state->cs, state->eip, state->eflags, state->err_code);
    for (;;)
        ;
}

//! single step
void I386::single_step_trap(I386::ISRFrame *const state)
{

    I386::console.printf("Single step");
    for (;;)
        ;
}

//! non maskable interrupt trap
void I386::nmi_trap(I386::ISRFrame *const state)
{

    I386::console.printf("NMI trap");
    for (;;)
        ;
}

//! breakpoint hit
void I386::breakpoint_trap(I386::ISRFrame *const state)
{

    I386::console.printf("Breakpoint trap");
    for (;;)
        ;
}

//! overflow
void I386::overflow_trap(I386::ISRFrame *const state)
{

    I386::console.printf("Overflow trap");
    for (;;)
        ;
}

//! bounds check
void I386::bounds_check_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Bounds check fault");
    for (;;)
        ;
}

//! invalid opcode / instruction
void I386::invalid_opcode_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Invalid opcode");
    for (;;)
        ;
}

//! device not available
void I386::no_device_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Device not found");
    for (;;)
        ;
}

//! double fault
void I386::double_fault_abort(I386::ISRFrame *const state)
{

    I386::console.printf("Double fault");
    for (;;)
        ;
}

//! invalid Task State Segment (TSS)
void I386::invalid_tss_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Invalid TSS");
    for (;;)
        ;
}

//! segment not present
void I386::no_segment_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Invalid segment");
    for (;;)
        ;
}

//! stack fault
void I386::stack_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Stack fault");
    for (;;)
        ;
}

//! general protection fault
void I386::general_protection_fault(I386::ISRFrame *const state)
{

    I386::console.printf("General Protection Fault");
    for (;;)
        ;
}

//! page fault
void I386::page_fault(I386::ISRFrame *const state)
{
    uint32_t addr;

    asm volatile("mov %%cr2, %0"
                 : "=a"(addr)
                 :
                 :);
    I386::console.printf("Page Fault at 0x%x:0x%x referenced memory at 0x%x", state->cs, state->eip, addr);
    for (;;)
        ;
}

//! Floating Point Unit (FPU) error
void I386::fpu_fault(I386::ISRFrame *const state)
{

    I386::console.printf("FPU Fault");
    for (;;)
        ;
}

//! alignment check
void I386::alignment_check_fault(I386::ISRFrame *const state)
{

    I386::console.printf("Alignment Check");
    for (;;)
        ;
}

//! machine check
void I386::machine_check_abort(I386::ISRFrame *const state)
{

    I386::console.printf("Machine Check");
    for (;;)
        ;
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void I386::simd_fpu_fault(I386::ISRFrame *const state)
{

    I386::console.printf("FPU SIMD fault");
    for (;;)
        ;
}
