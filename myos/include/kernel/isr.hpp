/**
 * This file contains the interface for Interrupt Vector Table (IVT) to 
 * handle hardware and software Interrput Requests (IRQ).
 * 
 * Author: Ketan Goyal
 */

#ifndef ISR_HPP
#define ISR_HPP

#include <stdint.h>

#include <kernel/defs.hpp>

#include <arch/isr.hpp>

/**
 * Maximum number of interrupt handlers supported by the kernel
 */
#define IVT_MAX_VECTORS 256

namespace kernel
{
    /**
     * ISR Stack Frame
     * 
     * The stack frame of ISR containing arch specific and non-specifc
     * data members.
     */
    struct __attribute__((packed)) ISRFrame
    {
        uint32_t n;   // interrupt number to be assigned to ISR
        ISRArg arg; // arch specific arguments for ISR
    };

    /** 
     * ISR function pointer typedef
     */
    typedef void (*isr_handler_t)(ISRFrame *const frame);

    /**
     * Interrupt Vector Table (IVT)
     * 
     * Used by the kernel to register Interrupt Service Request (ISR) handlers
     * in the IVT to handle software and hardware interupts.
     */
    namespace IVT
    {
        /**
         * Common interrupt handler entry point having the same signature as an 
         * interrupt handler.
         * 
         * NOTE: Arch specifc code should implementate interrupt entry stubs
         *      redirecting execution to this method.
         * 
         * @param frame pointer to interrupt stack frame
         */
        void isr_entry(ISRFrame *const frame);

        /**
         * Register Interrupt Service Routine (ISR) in Interrupt Vector Table (IVT).
         * 
         * @param n interrupt number to be assigned to ISR
         * @param isr function pointer to ISR
         */
        void register_isr(const uint32_t n, isr_handler_t isr);

    } // namespace IVT

} // namespace kernel

#endif /* ISR_HPP */
