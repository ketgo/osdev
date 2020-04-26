#include <stdint.h>

#include <arch/isr.hpp>
#include <boot/console.hpp>

#include <kernel/ivt.hpp>

boot::isr_handler_t kernel::IVT::vector[IVT_MAX_VECTORS];

static void isr_default_handler(boot::ISRFrame *const state)
{
    /** TODO: Use printk instead of console **/

    boot::console.set_bg_color(VGA_COLOR_BLUE);
    boot::console.set_fg_color(VGA_COLOR_WHITE);
    boot::console.clrscr();
    boot::console.printf("*** [ERROR] isr_default_handler: Unhandled Exception\n");
    boot::console.printf("\n----------- Stack ----------\n\n");
    boot::console.printf("IR: %d\n", state->int_num);
    boot::console.printf("\n----------------------------\n");

    // Stop system
    for (;;)
        ;
}

kernel::IVT::IVT()
{
    for (uint32_t i = 0; i < IVT_MAX_VECTORS; ++i)
        vector[i] = isr_default_handler;
}

void kernel::IVT::isr_entry(boot::ISRFrame *const state)
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

void kernel::IVT::register_isr(const uint32_t n, boot::isr_handler_t isr)
{
    if (n < IVT_MAX_VECTORS)
    {
        vector[n] = isr;
    }
}