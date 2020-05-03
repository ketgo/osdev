#include <stddef.h>
#include <stdint.h>

#include <arch/io.hpp>

void arch::cli()
{
    asm volatile("cli");
}

void arch::sti()
{
    asm volatile("sti");
}

void arch::rep_nop()
{
    asm volatile("rep; nop");
}

void arch::outb(uint8_t value, uint16_t port)
{
    asm volatile("outb %0,%1"
                 :
                 : "a"(value), "dN"(port));
}

uint8_t arch::inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %1,%0"
                 : "=a"(value)
                 : "dN"(port));
    return value;
}

void arch::outw(uint16_t value, uint16_t port)
{
    asm volatile("outw %0,%1"
                 :
                 : "a"(value), "dN"(port));
}

uint16_t arch::inw(uint16_t port)
{
    uint16_t value;
    asm volatile("inw %1,%0"
                 : "=a"(value)
                 : "dN"(port));
    return value;
}

void arch::outl(uint32_t value, uint16_t port)
{
    asm volatile("outl %0,%1"
                 :
                 : "a"(value), "dN"(port));
}

uint32_t arch::inl(uint16_t port)
{
    uint32_t value;
    asm volatile("inl %1,%0"
                 : "=a"(value)
                 : "dN"(port));
    return value;
}

void arch::io_delay()
{
    const uint16_t DELAY_PORT = 0x80;
    asm volatile("outb %%al,%0"
                 :
                 : "dN"(DELAY_PORT));
}