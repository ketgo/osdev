#include <stdint.h>

#include <kernel/panic.hpp>

#include <i386/exception.hpp>

//! divide by 0 fault
void I386::divide_by_zero_fault(kernel::ISRFrame *const frame)
{
    /** TODO: Incorrect parsing of %x in panic method. Not all digits are displayed at end of string. Temporarily added *** as quick fix. */
    kernel::panic("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] ***", frame->arg.cs, frame->arg.eip, frame->arg.eflags);
}

//! single step
void I386::single_step_trap(kernel::ISRFrame *const frame)
{
    kernel::panic("Single step");
}

//! non maskable interrupt trap
void I386::nmi_trap(kernel::ISRFrame *const frame)
{
    kernel::panic("NMI trap");
}

//! breakpoint hit
void I386::breakpoint_trap(kernel::ISRFrame *const frame)
{
    kernel::panic("Breakpoint trap");
}

//! overflow
void I386::overflow_trap(kernel::ISRFrame *const frame)
{
    kernel::panic("Overflow trap");
}

//! bounds check
void I386::bounds_check_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Bounds check fault");
}

//! invalid opcode / instruction
void I386::invalid_opcode_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Invalid opcode");
}

//! device not available
void I386::no_device_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Device not found");
}

//! double fault
void I386::double_fault_abort(kernel::ISRFrame *const frame)
{
    kernel::panic("Double fault");
}

//! invalid Task frame Segment (TSS)
void I386::invalid_tss_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Invalid TSS");
}

//! segment not present
void I386::no_segment_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Invalid segment");
}

//! stack fault
void I386::stack_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Stack fault");
}

//! general protection fault
void I386::general_protection_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("General Protection Fault");
}

//! page fault
void I386::page_fault(kernel::ISRFrame *const frame)
{
    uint32_t addr;

    asm volatile("mov %%cr2, %0"
                 : "=a"(addr)
                 :
                 :);
    kernel::panic("Page Fault at 0x%x:0x%x referenced memory at 0x%x", frame->arg.cs, frame->arg.eip, addr);
}

//! Floating Point Unit (FPU) error
void I386::fpu_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("FPU Fault");
}

//! alignment check
void I386::alignment_check_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("Alignment Check");
}

//! machine check
void I386::machine_check_abort(kernel::ISRFrame *const frame)
{
    kernel::panic("Machine Check");
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void I386::simd_fpu_fault(kernel::ISRFrame *const frame)
{
    kernel::panic("FPU SIMD fault");
}
