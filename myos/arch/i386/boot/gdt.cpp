#include <stdint.h>

#include <i386/gdt.hpp>

//---

/***********************
 * GDT Descriptor Class
 ***********************/

void I386::GDT::Descriptor::set_base(uint32_t base)
{
    base_lo = uint16_t(base & 0xffff);
    base_mid = uint8_t((base >> 16) & 0xff);
    base_hi = uint8_t((base >> 24) & 0xff);
}

uint32_t I386::GDT::Descriptor::get_base()
{
    uint32_t base;

    base = uint32_t(base_lo);
    base += uint32_t(base_mid) << 16;
    base += uint32_t(base_hi) << 24;

    return base;
}

void I386::GDT::Descriptor::set_limit(uint32_t limit)
{
    limit_lo = uint16_t(limit & 0xffff);
    flags |= uint8_t((limit >> 16) & 0x0f);
}

uint32_t I386::GDT::Descriptor::get_limit()
{
    uint32_t limit;

    limit = uint32_t(limit_lo);
    limit += uint32_t(flags & 0x0f) << 16;

    return limit;
}

void I386::GDT::Descriptor::set_access(uint8_t access)
{
    this->access = access;
}

uint8_t I386::GDT::Descriptor::get_access()
{
    return access;
}

void I386::GDT::Descriptor::set_flags(uint8_t flags)
{
    this->flags |= flags & 0xf0;
}

uint8_t I386::GDT::Descriptor::get_flags()
{
    return flags & 0xf0;
}

I386::GDT::Descriptor &I386::GDT::Descriptor::operator=(I386::GDT::Descriptor &other)
{
    if (this != &other) // self-assignment check
    {
        set_base(other.get_base());
        set_limit(other.get_limit());
        set_access(other.get_access());
        set_flags(other.get_flags());
    }
    return *this;
}

//---

/**
 * This struct describes a GDT pointer. It points to the start of our array of
 * GDT entries, and is in the format required by the lgdt instruction.
 */
static struct __attribute__((packed)) GDTRegister
{
    uint16_t limit; /**< Size of gdt table minus one. */
    uint32_t base;  /**< The base table address. */
} reg;

/**
 * Array of global descriptors.
 */
static I386::GDT::Descriptor gdt[GDT_MAX_DESCRIPTORS];

void I386::GDT::set_descriptor(uint32_t idx, I386::GDT::Descriptor *desc)
{
    if (idx >= GDT_MAX_DESCRIPTORS) // checks index
    {
        return;
    }
    gdt[idx] = *desc;
}

const I386::GDT::Descriptor *I386::GDT::get_descriptor(uint32_t idx)
{
    if (idx >= GDT_MAX_DESCRIPTORS) // checks index
    {
        return nullptr;
    }
    return &gdt[idx];
}

void I386::GDT::flush()
{
    reg.limit = sizeof(gdt) - 1;
    reg.base = (uint32_t)gdt;

    asm volatile("lgdtl   %0\n\t"
                 "movl    %1, %%eax\n\t"
                 "movl    %%eax, %%ds\n\t"
                 "movl    %%eax, %%es\n\t"
                 "movl    %%eax, %%fs\n\t"
                 "movl    %%eax, %%gs\n\t"
                 "movl    %%eax, %%ss\n\t"
                 "ljmpl    %2, $1f\n\t"
                 "1:\n\t"
                 :
                 : "m"(reg),
                   "i"((uint32_t)I386::GDT::KERNEL_DATA_SEGMENT),
                   "i"((uint32_t)I386::GDT::KERNEL_CODE_SEGMENT));
}

void I386::GDT::setup()
{
    /**
     * As required by x86 processors, set first descriptor as NULL 
     * descriptor.
     */
    gdt[0].set_base(0);
    gdt[0].set_limit(0);
    gdt[0].set_access(0);
    gdt[0].set_flags(0);

    /**
     * Setting default code descriptor.
     */
    gdt[1].set_base(0);
    gdt[1].set_limit(0xffffffff);
    gdt[1].set_access(GDT_DESC_ACCESS_CD_SEG | GDT_DESC_ACCESS_EXEC | GDT_DESC_ACCESS_RW | GDT_DESC_ACCESS_P);
    gdt[1].set_flags(GDT_DESC_FLAG_GR | GDT_DESC_FLAG_SZ);

    /**
     * Setting default data descriptor.
     */
    gdt[2].set_base(0);
    gdt[2].set_limit(0xffffffff);
    gdt[2].set_access(GDT_DESC_ACCESS_CD_SEG | GDT_DESC_ACCESS_RW | GDT_DESC_ACCESS_P);
    gdt[2].set_flags(GDT_DESC_FLAG_GR | GDT_DESC_FLAG_SZ);

    flush();
}
