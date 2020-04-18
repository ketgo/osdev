#include <stdint.h>

#include <boot/console.hpp>
#include <boot/io.hpp>
#include <boot/gdt.hpp>
#include <boot/idt.hpp>

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

/**
 * A struct describing a pointer to an array of interrupt handlers.
 * This is in a format suitable for giving to 'lidt'.
 */
struct __attribute__((packed)) IDTRegister
{
    uint16_t limit; /**< Size of idt table minus one. */
    uint32_t base;  /**< The first entry address. */
};

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
    static IDTRegister idt_reg;

    idt_reg.limit = sizeof(this->_idt) - 1;
    idt_reg.base = (uint32_t)this->_idt;

    asm volatile("lidtl   %0\n\t" ::"m"(idt_reg));
}

static void isr_default_handler()
{

    // Clear interrupts to prevent double fault
    boot::cli();

    boot::console.set_bg_color(VGA_COLOR_BLUE);
    boot::console.set_fg_color(VGA_COLOR_WHITE);
    boot::console.clrscr();
    boot::console.printf("*** [ERROR] isr_default_handler: Unhandled Exception");

    // Stop system
    for (;;)
        ;
}

void boot::IDT::initialize()
{
    for (int i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        this->_idt[i].set_handler((uint32_t)isr_default_handler);
        this->_idt[i].set_selector(boot::KERNEL_CODE_SEGMENT);
        this->_idt[i].set_flags(IDT_DESC_FLAG_PRESENT | IDT_DESC_FLAG_INT_BIT32);
    }

    this->flush();
}
