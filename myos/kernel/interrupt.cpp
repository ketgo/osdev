#include <stdint.h>

#include <arch/isr.hpp>

#include <kernel/panic.hpp>
#include <kernel/interrupt.hpp>

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
 * @param state pointer to ISR stack frame
 */
static void isr_default_handler(kernel::ISRFrame *const state)
{
    kernel::panic("*** [ERROR] isr_default_handler: Unhandled Exception\n");
}

void kernel::IVT::isr_entry(kernel::ISRFrame *const state)
{
    /** TODO: Stack handling for recursive interrupts */

    if (state->int_num < IVT_MAX_VECTORS)
    {
        if (vector[state->int_num] != nullptr)
        {
            vector[state->int_num](state);
        }
        else
        {
            isr_default_handler(state);
        }
    }
    else
    {
        isr_default_handler(state);
    }
}

void kernel::IVT::register_isr(const uint32_t n, kernel::isr_handler_t isr)
{
    if (n < IVT_MAX_VECTORS)
    {
        vector[n] = isr;
    }
}