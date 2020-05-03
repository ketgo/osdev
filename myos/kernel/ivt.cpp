#include <stdint.h>

#include <i386/isr.hpp>
#include <i386/console.hpp>

#include <kernel/ivt.hpp>

I386::isr_handler_t kernel::IVT::vector[IVT_MAX_VECTORS];

static void isr_default_handler(I386::ISRFrame *const state)
{
    /** TODO: Use printk instead of console **/

    I386::console.set_bg_color(I386::VGA_COLOR_BLUE);
    I386::console.set_fg_color(I386::VGA_COLOR_WHITE);
    I386::console.clrscr();
    I386::console.printf("*** [ERROR] isr_default_handler: Unhandled Exception\n");
    I386::console.printf("\n----------- Stack ----------\n\n");
    I386::console.printf("IR: %d\n", state->int_num);
    I386::console.printf("\n----------------------------\n");

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