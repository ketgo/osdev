#include <stdint.h>

#include <kernel/panic.hpp>
#include <kernel/isr.hpp>

/**
 * Array of interrupt vectors mapping interrupt numbers to corresponding
 * handlers.
 */
static kernel::isr_handler_t vector[IVT_MAX_VECTORS];

/**
 * Default interrupt handler
 * 
 * NOTE: Called when no interrupt handler is registered for the triggered
 *      interrupt number.
 * 
 * @param frame pointer to ISR stack frame
 */
static void isr_default_handler(kernel::ISRFrame *const frame)
{
    kernel::panic("*** [ERROR] isr_default_handler: Unhandled Interrupt [%d]", frame->n);
}

void kernel::IVT::isr_entry(kernel::ISRFrame *const frame)
{
    /** TODO: Stack handling for recursive interrupts */

    if (frame->n < IVT_MAX_VECTORS)
    {
        if (vector[frame->n] != nullptr)
        {
            vector[frame->n](frame);
            isr_exit(frame);
        }
        else
        {
            isr_default_handler(frame);
        }
    }
    else
    {
        isr_default_handler(frame);
    }
}

void kernel::IVT::register_isr(const uint32_t n, kernel::isr_handler_t isr)
{
    if (n < IVT_MAX_VECTORS)
    {
        vector[n] = isr;
    }
}
