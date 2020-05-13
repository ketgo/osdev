#include <stdint.h>

#include <i386/gdt.hpp>
#include <i386/idt.hpp>

#include <kernel/isr.hpp>

//---

/***********************
 * IDT Descriptor Class
 ***********************/

I386::IDT::Descriptor::Descriptor(uint32_t isr, uint16_t selector, uint8_t flags)
{
    set_isr(isr);
    set_selector(selector);
    set_flags(flags);
}

void I386::IDT::Descriptor::set_isr(uint32_t isr)
{
    offset_lo = uint16_t(isr & 0xffff);
    offset_hi = uint16_t((isr >> 16) & 0xffff);
}

uint32_t I386::IDT::Descriptor::get_isr()
{
    uint32_t isr_handler;

    isr_handler = uint32_t(offset_lo);
    isr_handler += uint32_t(offset_hi) << 16;

    return isr_handler;
}

void I386::IDT::Descriptor::set_selector(uint16_t selector)
{
    this->selector = selector;
}

uint16_t I386::IDT::Descriptor::get_selector()
{
    return selector;
}

void I386::IDT::Descriptor::set_flags(uint8_t flags)
{
    this->flags = flags;
}

uint8_t I386::IDT::Descriptor::get_flags()
{
    return flags;
}

I386::IDT::Descriptor &I386::IDT::Descriptor::operator=(I386::IDT::Descriptor &other)
{
    if (this != &other) // self-assignment check
    {
        set_isr(other.get_isr());
        set_selector(other.get_selector());
        set_flags(other.get_flags());
    }
    return *this;
}

//---

/**
 * A struct describing a pointer to an array of interrupt handlers.
 * This is in a format suitable for giving to 'lidt'.
 */
static struct __attribute__((packed)) IDTRegister
{
    uint16_t limit; /**< Size of idt table minus one. */
    uint32_t base;  /**< The first entry address. */
} reg;

/**
 * Array of descriptors
 */
static I386::IDT::Descriptor idt[IDT_MAX_DESCRIPTORS]; /**< Array of descriptors. */

/**
 * Interrupt Service Request (ISR) entry and exit stub template. 
 * 
 * The template sets up the stack frame for an ISR. This is needed since 
 * the standard C++ stack frame setup for functions is inconsistent with 
 * that required for interrupts due to the difference in the ret and iret
 * assembly instructions. After setting up the stack frame, the ISR stub 
 * redirects execution to the common interrupt handler entry point 
 * `kernel::IVT::isr_entry`.
 * 
 * @param n interrupt number associated with ISR
 * @param error_code boolean indcating interrupt is passed with error code. 
 *  If not a bogus error code is passed to ISR handler stack frame.
 */
template <uint32_t num, bool error_code>
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
                 // Push interrupt number to stack frame
                 "push %1\n\t"
                 // Push a pointer to an stack frame used by the ISR handler common entry point
                 "push %%esp\n\t"
                 // Call the ISR handler common entry point
                 "mov %3, %%eax\n\t"
                 "call *%%eax\n\t"
                 // Unwind the stack by reducing stack size by the 8 bytes
                 // This is done to unload the stack frame pointer and isr number
                 "add  $8, %%esp\n\t"
                 // Reload the original data segment descriptor
                 "pop  %%eax\n\t"
                 "mov  %%ax, %%ds\n\t"
                 "mov  %%ax, %%es\n\t"
                 "mov  %%ax, %%fs\n\t"
                 "mov  %%ax, %%gs\n\t"
                 // Pop edi,esi,ebp,esp,ebx,edx,ecx,eax
                 "popa\n\t"
                 // Unload the pushed error code
                 "add  $4, %%esp\n\t"
                 // Enable interrupts
                 "sti\n\t"
                 // pops 5 things at once: cs,eip,eflags,ss,esp
                 "iret\n\t"
                 :
                 : "i"(error_code),
                   "i"(num),
                   "i"(I386::GDT::KERNEL_DATA_SEGMENT),
                   "i"(kernel::IVT::isr_entry));
}

void I386::IDT::set_descriptor(uint32_t idx, I386::IDT::Descriptor *desc)
{
    if (idx >= IDT_MAX_DESCRIPTORS) // checks index
    {
        return;
    }
    idt[idx] = *desc;
}

const I386::IDT::Descriptor *I386::IDT::get_descriptor(uint32_t idx)
{
    if (idx >= IDT_MAX_DESCRIPTORS) // checks index
    {
        return nullptr;
    }
    return &idt[idx];
}

void I386::IDT::flush()
{
    reg.limit = sizeof(idt) - 1;
    reg.base = (uint32_t)idt;

    asm volatile("lidtl   %0\n\t" ::"m"(reg));
}

void I386::IDT::setup()
{
#define SETUP_IRQ(num, error_code)                          \
    idt[num].set_isr((uint32_t)&isr_stub<num, error_code>); \
    idt[num].set_selector(I386::GDT::KERNEL_CODE_SEGMENT);  \
    idt[num].set_flags(IDT_DESC_FLAG_PRESENT | IDT_DESC_FLAG_INT_BIT32);

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

    SETUP_IRQ(32, false);
    SETUP_IRQ(33, false);
    SETUP_IRQ(34, false);
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

    SETUP_IRQ(ISR_SYSCALL, false); // Commonly used IRQ number used for syscalls.

#undef SETUP_IRQ

    flush();
}
