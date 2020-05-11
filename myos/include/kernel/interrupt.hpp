/**
 * This file contains the interface for Interrupt Vector Table (IVT) to 
 * handle hardware and software Interrput Requests (IRQ).
 * 
 * Author: Ketan Goyal
 */

#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

#include <stdint.h>

#include <arch/isr.hpp>

/**
 * Maximum number of interrupt handlers supported by the kernel
 */
#define IVT_MAX_VECTORS 256

namespace kernel
{

    /**
     * Interrupt Vector Table (IVT)
     * 
     * Used by the kernel to register Interrupt Service Request (ISR) handlers
     * in the IVT to handle software and hardware interupts.
     */
    namespace IVT
    {

        /**
         * Common interrupt handler entry point. This method has the same
         * signature as an interrupt handler.
         * 
         * NOTE: Arch specifc code should implementate interrupt entry stubs
         *      redirecting execution to this method.
         * 
         * @param state pointer to interrupt stack frame containing CPU state
         */
        void isr_entry(ISRFrame *const state);

        /**
         * Register Interrupt Service Routine (ISR) in Interrupt Vector Table (IVT).
         * 
         * @param n interrupt number to be assigned to ISR
         * @param isr function pointer to ISR
         */
        void register_isr(const uint32_t n, isr_handler_t isr);

    } // namespace IVT

} // namespace kernel

#endif /* INTERRUPT_HPP */
