#include <stdint.h>

#include <arch/isr.hpp>

#include <kernel/panic.hpp>
#include <kernel/interrupt.hpp>

kernel::isr_handler_t kernel::IVT::vector[IVT_MAX_VECTORS];

static void isr_default_handler(kernel::ISRFrame *const state)
{
    kernel::panic("*** [ERROR] isr_default_handler: Unhandled Exception\n");
}

kernel::IVT::IVT()
{
    for (uint32_t i = 0; i < IVT_MAX_VECTORS; ++i)
        vector[i] = isr_default_handler;
}

void kernel::IVT::isr_entry(kernel::ISRFrame *const state)
{
    /** TODO: Stack handling for recursive interrupts */

    if (state->int_num < IVT_MAX_VECTORS)
    {
        vector[state->int_num](state);
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