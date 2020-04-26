#include <stdint.h>

#include <boot/console.hpp>
#include <boot/gdt.hpp>
#include <boot/idt.hpp>

#include <arch/io.hpp>
#include <arch/isr.hpp>

/***********************
 * IDT Descriptor Class
 ***********************/

boot::IDTDescriptor::IDTDescriptor(uint32_t isr, uint16_t selector, uint8_t flags)
{
    set_isr(isr);
    set_selector(selector);
    set_flags(flags);
}

void boot::IDTDescriptor::set_isr(uint32_t isr)
{
    offset_lo = uint16_t(isr & 0xffff);
    offset_hi = uint16_t((isr >> 16) & 0xffff);
}

uint32_t boot::IDTDescriptor::get_isr()
{
    uint32_t isr_handler;

    isr_handler = uint32_t(offset_lo);
    isr_handler += uint32_t(offset_hi) << 16;

    return isr_handler;
}

void boot::IDTDescriptor::set_selector(uint16_t selector)
{
    this->selector = selector;
}

uint16_t boot::IDTDescriptor::get_selector()
{
    return selector;
}

void boot::IDTDescriptor::set_flags(uint8_t flags)
{
    this->flags = flags;
}

uint8_t boot::IDTDescriptor::get_flags()
{
    return flags;
}

boot::IDTDescriptor &boot::IDTDescriptor::operator=(boot::IDTDescriptor &other)
{
    if (this != &other) // self-assignment check
    {
        set_isr(other.get_isr());
        set_selector(other.get_selector());
        set_flags(other.get_flags());
    }
    return *this;
}

/********************
 * IDT Class
 *******************/

boot::IDT boot::idt;

void boot::IDT::flush()
{
    idt_reg.limit = sizeof(_idt) - 1;
    idt_reg.base = (uint32_t)_idt;

    asm volatile("lidtl   %0\n\t" ::"m"(idt_reg));
}

template <uint32_t num, bool error_code>
void boot::IDT::isr_stub(void)
{
    asm volatile("cli\n\t"
                 // Check if the sub is for exception interrupt
                 ".if %c0\n\t"
                 // Do nothing as the error code is pushed by processor.
                 ".else\n\t"
                 // Else push bogus error code
                 "push $0\n\t"
                 // End of if marco
                 ".endif\n\t"
                 // Push interrupt number to stack frame
                 "push %1\n\t"
                 // Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
                 "pusha\n\t"
                 // Get the original data segment descriptor
                 "mov %%ds, %%ax\n\t"
                 // And save it into the stack
                 "push %%eax\n\t"
                 // Load the kernel data segment descriptor
                 "mov %2, %%ax\n\t"
                 "mov %%ax, %%ds\n\t"
                 "mov %%ax, %%es\n\t"
                 "mov %%ax, %%fs\n\t"
                 "mov %%ax, %%gs\n\t"
                 // Push a pointer to an isr_frame struct
                 "push %%esp\n\t"
                 // Call ISR handler common entry point
                 "mov %3, %%eax\n\t"
                 "call %%eax\n\t"
                 // Unwind the stack by reducing stack size by the size of pointer
                 // to isr_frame struct. This is done to unload the isr_frame struct
                 // pointer pushed to stack earlier.
                 "add  $4, %%esp\n\t"
                 // Reload the original data segment descriptor
                 "pop  %%eax\n\t"
                 "mov  %%ax, %%ds\n\t"
                 "mov  %%ax, %%es\n\t"
                 "mov  %%ax, %%fs\n\t"
                 "mov  %%ax, %%gs\n\t"
                 // Pop edi,esi,ebp,esp,ebx,edx,ecx,eax
                 "popa\n\t"
                 // Clean up the pushed error code and isr number
                 "add  $8, %%esp\n\t"
                 // Enable interrupts
                 "sti\n\t"
                 // pops 5 things at once: cs,eip,eflags,ss,esp
                 "iret\n\t"
                 :
                 : "i"(error_code),
                   "i"(num),
                   "i"(boot::KERNEL_DATA_SEGMENT),
                   "i"(isr_entry));
}

/**
 * Template to setup multiple ISR handlers.
 * 
 */

void boot::IDT::setup()
{
#define SETUP_IRQ(num, error_code)                                      \
    _idt[num].set_isr((uint32_t)&boot::IDT::isr_stub<num, error_code>); \
    _idt[num].set_selector(boot::KERNEL_CODE_SEGMENT);                  \
    _idt[num].set_flags(IDT_DESC_FLAG_PRESENT | IDT_DESC_FLAG_INT_BIT32);

    /*
    * Setup interrupts for Exceptions and Faults
    */

    SETUP_IRQ(0, false);
    SETUP_IRQ(1, false);
    SETUP_IRQ(2, false);
    SETUP_IRQ(3, false);
    SETUP_IRQ(4, false);
    SETUP_IRQ(5, false);
    SETUP_IRQ(6, false);
    SETUP_IRQ(7, false);
    SETUP_IRQ(8, true);
    SETUP_IRQ(9, false);
    SETUP_IRQ(10, true);
    SETUP_IRQ(11, true);
    SETUP_IRQ(12, true);
    SETUP_IRQ(13, true);
    SETUP_IRQ(14, true);
    SETUP_IRQ(15, false);
    SETUP_IRQ(16, false);
    SETUP_IRQ(17, false);
    SETUP_IRQ(18, false);
    SETUP_IRQ(19, false);
    SETUP_IRQ(20, false);
    SETUP_IRQ(21, true);
    SETUP_IRQ(21, true);
    SETUP_IRQ(22, true);
    SETUP_IRQ(23, true);
    SETUP_IRQ(24, true);
    SETUP_IRQ(25, false);
    SETUP_IRQ(26, false);
    SETUP_IRQ(27, false);
    SETUP_IRQ(28, false);
    SETUP_IRQ(29, false);
    SETUP_IRQ(30, false);
    SETUP_IRQ(31, false);

    /**
     * Setup default handlers for rest of the interrupts
     */

    SETUP_IRQ(32, true);
    SETUP_IRQ(33, true);
    SETUP_IRQ(34, true);
    SETUP_IRQ(35, false);
    SETUP_IRQ(36, false);
    SETUP_IRQ(37, false);
    SETUP_IRQ(38, false);
    SETUP_IRQ(39, false);
    SETUP_IRQ(40, false);
    SETUP_IRQ(41, false);
    SETUP_IRQ(42, false);
    SETUP_IRQ(43, false);
    SETUP_IRQ(44, false);
    SETUP_IRQ(45, false);
    SETUP_IRQ(46, false);
    SETUP_IRQ(47, false);

    /** 
     * Software interrupt (used by syscalls) 
     */

    SETUP_IRQ(128, false); // Commonly used IRQ number used for syscalls.

#undef SETUP_IRQ

    flush();
}
