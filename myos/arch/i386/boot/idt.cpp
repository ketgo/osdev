#include <stdint.h>

#include <boot/console.hpp>
#include <boot/io.hpp>
#include <boot/gdt.hpp>
#include <boot/idt.hpp>
#include <boot/isr.hpp>

/***********************
 * IDT Descriptor Class
 ***********************/

boot::IDTDescriptor::IDTDescriptor(uint32_t isr, uint16_t selector, uint8_t flags)
{
    set_isr(isr);
    set_selector(selector);
    set_flags(flags);
}

void boot::IDTDescriptor::set_isr(uint32_t isr_handler)
{
    offset_lo = uint16_t(isr_handler & 0xffff);
    offset_hi = uint16_t((isr_handler >> 16) & 0xffff);
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

void boot::IDT::set_descriptor(uint32_t n, IDTDescriptor *idt_desc)
{
    if (n > IDT_MAX_DESCRIPTORS) // checks index
    {
        return;
    }

    _idt[n] = *idt_desc;
}

const boot::IDTDescriptor *boot::IDT::get_descriptor(uint32_t n)
{
    if (n > IDT_MAX_DESCRIPTORS) // checks index
    {
        return 0;
    }

    return &_idt[n];
}

void boot::IDT::flush()
{
    idt_reg.limit = sizeof(_idt) - 1;
    idt_reg.base = (uint32_t)_idt;

    asm volatile("lidtl   %0\n\t" ::"m"(idt_reg));
}

/**
 * Interrupt Service Request (ISR) entry and exit stub template. 
 * 
 * The template sets up the stack frame for an ISR. This is needed since 
 * the standard C++ stack frame setup for functions is inconsistent with 
 * that required for interrupts. The underlying assembly instructions for
 * a standard function call uses ret to return while an interrupt uses iret.
 * 
 * @param num interrupt number associated with ISR
 * @param is_exception boolean indcating interrupt handles exception. If not 
 *  a bogus error code is pushed passed to ISR handler stack frame.
 * @param isr_handler pointer to ISR handler
 */
template <uint32_t num, bool is_exception, boot::isr_handler_t isr_entry>
static void isr_stub(void)
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
                 : "i"(is_exception),
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
#define REGISTER_ISR(num, is_exception)                   \
    boot::IDTDescriptor idt_desc(                         \
        (uint32_t)isr_stub<5, is_exception, isr_entry>, \
        boot::KERNEL_CODE_SEGMENT,                        \
        IDT_DESC_FLAG_PRESENT | IDT_DESC_FLAG_INT_BIT32); \
    set_descriptor(i, &idt_desc);

    for (int i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        REGISTER_ISR(i, false);
    }

#undef REGISTER_ISR

    flush();
}
