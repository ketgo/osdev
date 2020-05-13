/**
 * x86 system exception handlers
 */

#ifndef ARCH_I386_EXCEPTION_HPP
#define ARCH_I386_EXCEPTION_HPP

#include <kernel/isr.hpp>

namespace I386
{

    /**
     * Divide by zero fault
     */
    void divide_by_zero_fault(kernel::ISRFrame *const frame);

    /** single step */
    void single_step_trap(kernel::ISRFrame *const frame);

    /** non maskable interrupt trap */
    void nmi_trap(kernel::ISRFrame *const frame);

    /** breakpoint hit */
    void breakpoint_trap(kernel::ISRFrame *const frame);

    /** overflow */
    void overflow_trap(kernel::ISRFrame *const frame);

    /** bounds check */
    void bounds_check_fault(kernel::ISRFrame *const frame);

    /** invalid opcode / instruction */
    void invalid_opcode_fault(kernel::ISRFrame *const frame);

    /** device not available */
    void no_device_fault(kernel::ISRFrame *const frame);

    /** double fault */
    void double_fault_abort(kernel::ISRFrame *const frame);

    /** invalid Task frame Segment (TSS) */
    void invalid_tss_fault(kernel::ISRFrame *const frame);

    /** segment not present */
    void no_segment_fault(kernel::ISRFrame *const frame);

    /** stack fault */
    void stack_fault(kernel::ISRFrame *const frame);

    /** general protection fault */
    void general_protection_fault(kernel::ISRFrame *const frame);

    /** page fault */
    void page_fault(kernel::ISRFrame *const frame);

    /** Floating Point Unit (FPU) error */
    void fpu_fault(kernel::ISRFrame *const frame);

    /** alignment check */
    void alignment_check_fault(kernel::ISRFrame *const frame);

    /** machine check */
    void machine_check_abort(kernel::ISRFrame *const frame);

    /** Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error */
    void simd_fpu_fault(kernel::ISRFrame *const frame);

} // namespace I386

#endif /* ARCH_I386_EXCEPTION_HPP */
