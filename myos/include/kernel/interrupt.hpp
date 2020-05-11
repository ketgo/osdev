/**
 * This file contains the interface for arch specific code base to create 
 * implementation of Interrupt Vector Table (IVT) and Interrupt Service
 * Routines (ISR) to handle hardware and software Interrput Requests (IRQ).
 * 
 * Author: Ketan Goyal
 */

#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

#include <stdint.h>

#include <arch/isr.hpp>

namespace kernel
{

/**
 * Maximum number of interrupt handlers supported by the kernel
 */
#define IVT_MAX_VECTORS 256

/**
 * Interface for Interrupt Vector Table (IVT) to be implemented by arch specific
 * code base. The interface is used by the kernel to register Interrupt Service
 * Requests (ISR) in the IVT to handle software and hardware interupts.
 */
class IVT
{
private:
    /**
     * Array of interrupt vectors, a.k.a interrupt handlers.
     */
    static isr_handler_t vector[IVT_MAX_VECTORS];

protected:
    /**
     * Common interrupt handler entry point. This method has the same
     * signature as an interrupt handler.
     * 
     * @param state pointer to interrupt stack frame containing CPU state
     */
    static void isr_entry(ISRFrame *const state);

public:
    /** Default constructor */
    IVT();

    /**
     * Register Interrupt Service Routine (ISR) in Interrupt Vector Table (IVT).
     * 
     * @param n interrupt number to be assigned to ISR
     * @param isr function pointer to ISR
     */
    static void register_isr(const uint32_t n, isr_handler_t isr);
};

} // namespace kernel

#endif /* INTERRUPT_HPP */
