#include <stdint.h>

#include <i386/isr.hpp>
#include <kernel/console.hpp>

#include <kernel/ivt.hpp>

I386::isr_handler_t kernel::IVT::vector[IVT_MAX_VECTORS];

static void isr_default_handler(I386::ISRFrame *const state)
{
    /** TODO: Use panic instead of console **/

    kernel::console.set_bg_color(kernel::VGA_COLOR_BLUE);
    kernel::console.set_fg_color(kernel::VGA_COLOR_WHITE);
    kernel::console.clrscr();
    kernel::console.printf("*** [ERROR] isr_default_handler: Unhandled Exception\n");
    kernel::console.printf("\n----------- Stack ----------\n\n");
    kernel::console.printf("IR: %d\n", state->int_num);
    kernel::console.printf("\n----------------------------\n");

    // Stop system
    for (;;)
        ;
}

kernel::IVT::IVT()
{
    for (uint32_t i = 0; i < IVT_MAX_VECTORS; ++i)
        vector[i] = isr_default_handler;
}

void kernel::IVT::isr_entry(I386::ISRFrame *const state)
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

void kernel::IVT::register_isr(const uint32_t n, I386::isr_handler_t isr)
{
    if (n < IVT_MAX_VECTORS)
    {
        vector[n] = isr;
    }
}