/**
 * x86 system exception handlers
 */

#ifndef ARCH_I386_EXCEPTION_HPP
#define ARCH_I386_EXCEPTION_HPP

#include <i386/isr.hpp>

namespace I386
{

/**
 * Divide by zero fault
 */
void divide_by_zero_fault(ISRFrame *const state);

/** single step */
void single_step_trap(ISRFrame *const state);

/** non maskable interrupt trap */
void nmi_trap(ISRFrame *const state);

/** breakpoint hit */
void breakpoint_trap(ISRFrame *const state);

/** overflow */
void overflow_trap(ISRFrame *const state);

/** bounds check */
void bounds_check_fault(ISRFrame *const state);

/** invalid opcode / instruction */
void invalid_opcode_fault(ISRFrame *const state);

/** device not available */
void no_device_fault(ISRFrame *const state);

/** double fault */
void double_fault_abort(ISRFrame *const state);

/** invalid Task State Segment (TSS) */
void invalid_tss_fault(ISRFrame *const state);

/** segment not present */
void no_segment_fault(ISRFrame *const state);

/** stack fault */
void stack_fault(ISRFrame *const state);

/** general protection fault */
void general_protection_fault(ISRFrame *const state);

/** page fault */
void page_fault(ISRFrame *const state);

/** Floating Point Unit (FPU) error */
void fpu_fault(ISRFrame *const state);

/** alignment check */
void alignment_check_fault(ISRFrame *const state);

/** machine check */
void machine_check_abort(ISRFrame *const state);

/** Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error */
void simd_fpu_fault(ISRFrame *const state);

} // namespace I386

#endif /* ARCH_I386_EXCEPTION_HPP */
