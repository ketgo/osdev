/**
 * This file contains the interface for arch specific code base to create 
 * implementation of Interrupt Vector Table (IVT) and Interrupt Service
 * Routines (ISR) to handle hardware and software Interrput Requests (IRQ).
 * 
 * Author: Ketan Goyal
 */

#ifndef IRQ_HPP
#define IRQ_HPP

#include <stdint.h>

/**
 * Function pointer type to ISR handling inturrept requests (IRQ).
 */
typedef void (*irq_handler_t)(void);

namespace kernel
{

/**
 * Interface for Interrupt Vector Table (IVT) to be implemented by arch specific
 * code base. The interface is used by the kernel to register Interrupt Service
 * Requests (ISR) in the IVT to handle software and hardware interupts.
 */
class IVT
{
public:
    /**
     * Register Interrupt Service Routine (ISR) in Interrupt Vector Table (IVT).
     * 
     * @param n interrupt number to be assigned to ISR
     * @param isr function pointer to ISR
     */
    virtual void register_isr(const uint32_t n, irq_handler_t isr) = 0;
};

/**
 * IVT object pointer used by kernel. The instance should to be declared by the
 * arch specific codebase.
 */
extern IVT *ivt;
} // namespace kernel

#endif /* IRQ_HPP */
