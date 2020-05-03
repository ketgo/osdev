/**
 * Basic assembly routines
 */

#ifndef ARCH_I386_ASM_HPP
#define ARCH_I386_ASM_HPP

#include <stdint.h>

namespace I386
{
/* These functions are used to reference data in other segments. */

static inline uint16_t ds(void)
{
    uint16_t seg;
    asm("movw %%ds,%0"
        : "=rm"(seg));
    return seg;
}

static inline void set_fs(uint16_t seg)
{
    asm volatile("movw %0,%%fs"
                 :
                 : "rm"(seg));
}

static inline uint16_t fs(void)
{
    uint16_t seg;
    asm volatile("movw %%fs,%0"
                 : "=rm"(seg));
    return seg;
}

static inline void set_gs(uint16_t seg)
{
    asm volatile("movw %0,%%gs"
                 :
                 : "rm"(seg));
}

static inline uint16_t gs(void)
{
    uint16_t seg;
    asm volatile("movw %%gs,%0"
                 : "=rm"(seg));
    return seg;
}

typedef unsigned int addr_t;

static inline uint8_t rdfs8(addr_t addr)
{
    uint8_t v;
    asm volatile("movb %%fs:%1,%0"
                 : "=q"(v)
                 : "m"(*(uint8_t *)addr));
    return v;
}

static inline uint16_t rdfs16(addr_t addr)
{
    uint16_t v;
    asm volatile("movw %%fs:%1,%0"
                 : "=r"(v)
                 : "m"(*(uint16_t *)addr));
    return v;
}

static inline uint32_t rdfs32(addr_t addr)
{
    uint32_t v;
    asm volatile("movl %%fs:%1,%0"
                 : "=r"(v)
                 : "m"(*(uint32_t *)addr));
    return v;
}

static inline void wrfs8(uint8_t v, addr_t addr)
{
    asm volatile("movb %1,%%fs:%0"
                 : "+m"(*(uint8_t *)addr)
                 : "qi"(v));
}

static inline void wrfs16(uint16_t v, addr_t addr)
{
    asm volatile("movw %1,%%fs:%0"
                 : "+m"(*(uint16_t *)addr)
                 : "ri"(v));
}

static inline void wrfs32(uint32_t v, addr_t addr)
{
    asm volatile("movl %1,%%fs:%0"
                 : "+m"(*(uint32_t *)addr)
                 : "ri"(v));
}

static inline uint8_t rdgs8(addr_t addr)
{
    uint8_t v;
    asm volatile("movb %%gs:%1,%0"
                 : "=q"(v)
                 : "m"(*(uint8_t *)addr));
    return v;
}

static inline uint16_t rdgs16(addr_t addr)
{
    uint16_t v;
    asm volatile("movw %%gs:%1,%0"
                 : "=r"(v)
                 : "m"(*(uint16_t *)addr));
    return v;
}

static inline uint32_t rdgs32(addr_t addr)
{
    uint32_t v;
    asm volatile("movl %%gs:%1,%0"
                 : "=r"(v)
                 : "m"(*(uint32_t *)addr));
    return v;
}

static inline void wrgs8(uint8_t v, addr_t addr)
{
    asm volatile("movb %1,%%gs:%0"
                 : "+m"(*(uint8_t *)addr)
                 : "qi"(v));
}

static inline void wrgs16(uint16_t v, addr_t addr)
{
    asm volatile("movw %1,%%gs:%0"
                 : "+m"(*(uint16_t *)addr)
                 : "ri"(v));
}

static inline void wrgs32(uint32_t v, addr_t addr)
{
    asm volatile("movl %1,%%gs:%0"
                 : "+m"(*(uint32_t *)addr)
                 : "ri"(v));
}

} // namespace I386

#endif /* ARCH_I386_ASM_HPP */
