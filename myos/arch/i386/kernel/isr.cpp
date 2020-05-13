#include <kernel/isr.hpp>

#include <i386/pic.hpp>

void kernel::IVT::isr_exit(ISRFrame *const frame)
{
    I386::PIC::eoi(frame->n);
}
