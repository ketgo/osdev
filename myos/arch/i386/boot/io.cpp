#include <stddef.h>
#include <stdint.h>

#include <boot/io.hpp>

inline void boot::cli()
{
    asm volatile("cli");
}

inline void boot::sti()
{
    asm volatile("sti");
}

inline void boot::rep_nop()
{
    asm volatile("rep; nop");
}

inline void boot::outb(uint8_t value, uint16_t port)
{
    asm volatile("outb %0,%1"
                 :
                 : "a"(value), "dN"(port));
}

inline uint8_t boot::inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %1,%0"
                 : "=a"(value)
                 : "dN"(port));
    return value;
}

inline void boot::outw(uint16_t value, uint16_t port)
{
    asm volatile("outw %0,%1"
                 :
                 : "a"(value), "dN"(port));
}

inline uint16_t boot::inw(uint16_t port)
{
    uint16_t value;
    asm volatile("inw %1,%0"
                 : "=a"(value)
                 : "dN"(port));
    return value;
}

inline void boot::outl(uint32_t value, uint16_t port)
{
    asm volatile("outl %0,%1"
                 :
                 : "a"(value), "dN"(port));
}

inline uint32_t boot::inl(uint16_t port)
{
    uint32_t value;
    asm volatile("inl %1,%0"
                 : "=a"(value)
                 : "dN"(port));
    return value;
}

inline void boot::io_delay()
{
    const uint16_t DELAY_PORT = 0x80;
    asm volatile("outb %%al,%0"
                 :
                 : "dN"(DELAY_PORT));
}