#include <stdint.h>

#include <kernel/panic.hpp>
#include <arch/isr.hpp>
#include <i386/exception.hpp>

//! divide by 0 fault
void I386::divide_by_zero_fault(kernel::ISRFrame *const state)
{

    kernel::panic("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] Error Code: 0x%x", state->cs, state->eip, state->eflags, state->err_code);
}

//! single step
void I386::single_step_trap(kernel::ISRFrame *const state)
{

    kernel::panic("Single step");
}

//! non maskable interrupt trap
void I386::nmi_trap(kernel::ISRFrame *const state)
{

    kernel::panic("NMI trap");
}

//! breakpoint hit
void I386::breakpoint_trap(kernel::ISRFrame *const state)
{
    kernel::panic("Breakpoint trap");
}

//! overflow
void I386::overflow_trap(kernel::ISRFrame *const state)
{
    kernel::panic("Overflow trap");
}

//! bounds check
void I386::bounds_check_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Bounds check fault");
}

//! invalid opcode / instruction
void I386::invalid_opcode_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Invalid opcode");
}

//! device not available
void I386::no_device_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Device not found");
}

//! double fault
void I386::double_fault_abort(kernel::ISRFrame *const state)
{
    kernel::panic("Double fault");
}

//! invalid Task State Segment (TSS)
void I386::invalid_tss_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Invalid TSS");
}

//! segment not present
void I386::no_segment_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Invalid segment");
}

//! stack fault
void I386::stack_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Stack fault");
}

//! general protection fault
void I386::general_protection_fault(kernel::ISRFrame *const state)
{
    kernel::panic("General Protection Fault");
}

//! page fault
void I386::page_fault(kernel::ISRFrame *const state)
{
    uint32_t addr;

    asm volatile("mov %%cr2, %0"
                 : "=a"(addr)
                 :
                 :);
    kernel::panic("Page Fault at 0x%x:0x%x referenced memory at 0x%x", state->cs, state->eip, addr);
}

//! Floating Point Unit (FPU) error
void I386::fpu_fault(kernel::ISRFrame *const state)
{
    kernel::panic("FPU Fault");
}

//! alignment check
void I386::alignment_check_fault(kernel::ISRFrame *const state)
{
    kernel::panic("Alignment Check");
}

//! machine check
void I386::machine_check_abort(kernel::ISRFrame *const state)
{
    kernel::panic("Machine Check");
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void I386::simd_fpu_fault(kernel::ISRFrame *const state)
{
    kernel::panic("FPU SIMD fault");
}
