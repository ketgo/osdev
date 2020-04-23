#include <stdint.h>

#include <boot/console.hpp>
#include <boot/io.hpp>
#include <boot/gdt.hpp>
#include <boot/idt.hpp>
#include <boot/isr.hpp>

/***********************
 * IDT Descriptor Class
 ***********************/

void boot::IDTDescriptor::set_handler(uint32_t isr_handler)
{
    this->offset_lo = uint16_t(isr_handler & 0xffff);
    this->offset_hi = uint16_t((isr_handler >> 16) & 0xffff);
}

uint32_t boot::IDTDescriptor::get_handler()
{
    uint32_t isr_handler;

    isr_handler = uint32_t(this->offset_lo);
    isr_handler += uint32_t(this->offset_hi) << 16;

    return isr_handler;
}

void boot::IDTDescriptor::set_selector(uint16_t selector)
{
    this->selector = selector;
}

uint16_t boot::IDTDescriptor::get_selector()
{
    return this->selector;
}

void boot::IDTDescriptor::set_flags(uint8_t flags)
{
    this->flags = flags;
}

uint8_t boot::IDTDescriptor::get_flags()
{
    return this->flags;
}

boot::IDTDescriptor &boot::IDTDescriptor::operator=(boot::IDTDescriptor &other)
{
    if (this != &other) // self-assignment check
    {
        this->set_handler(other.get_handler());
        this->set_selector(other.get_selector());
        this->set_flags(other.get_flags());
    }
    return *this;
}

/********************
 * IDT Class
 *******************/

boot::IDT boot::idt;

void boot::IDT::set_descriptor(uint32_t n, boot::IDTDescriptor *idt_desc)
{
    if (n > IDT_MAX_DESCRIPTORS) // checks index
    {
        return;
    }

    this->_idt[n] = *idt_desc;
}

boot::IDTDescriptor *boot::IDT::get_descriptor(uint32_t n)
{
    if (n > IDT_MAX_DESCRIPTORS) // checks index
    {
        return 0;
    }

    return &this->_idt[n];
}

void boot::IDT::flush()
{
    this->idt_reg.limit = sizeof(this->_idt) - 1;
    this->idt_reg.base = (uint32_t)this->_idt;

    asm volatile("lidtl   %0\n\t" ::"m"(this->idt_reg));
}

extern "C" void isr_default_handler(boot::ISRFrame *const state)
{
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
template <uint32_t num, bool is_exception = false, boot::isr_handler_t isr_handler = isr_default_handler>
static void isr_stub(void)
{
    asm volatile("cli\n\t"
                 // Check if the sub is for exception interrupt
                 ".if %c0\n\t"
                 // Do nothing as the error code is pushed by processor.
                 ".else\n\t"
                 // Push bogus error code
                 "push $0\n\t"
                 // End if marco
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
                   "i"(isr_handler));
}

void boot::IDT::initialize()
{
    for (int i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        const int num = 5;
        this->_idt[i].set_handler((uint32_t)isr_stub<num>);
        this->_idt[i].set_selector(boot::KERNEL_CODE_SEGMENT);
        this->_idt[i].set_flags(IDT_DESC_FLAG_PRESENT | IDT_DESC_FLAG_INT_BIT32);
    }

    this->flush();
}
