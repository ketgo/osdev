#include <stdint.h>

#include <boot/gdt.hpp>

boot::GDT boot::gdt;

void boot::GDTDescriptor::set_base(uint32_t base)
{
    base_lo = uint16_t(base & 0xffff);
    base_mid = uint8_t((base >> 16) & 0xff);
    base_hi = uint8_t((base >> 24) & 0xff);
}

uint32_t boot::GDTDescriptor::get_base()
{
    uint32_t base;

    base = uint32_t(base_lo);
    base += uint32_t(base_mid) << 16;
    base += uint32_t(base_hi) << 24;

    return base;
}

void boot::GDTDescriptor::set_limit(uint32_t limit)
{
    limit_lo = uint16_t(limit & 0xffff);
    flags |= uint8_t((limit >> 16) & 0x0f);
}

uint32_t boot::GDTDescriptor::get_limit()
{
    uint32_t limit;

    limit = uint32_t(limit_lo);
    limit += uint32_t(flags & 0x0f) << 16;

    return limit;
}

void boot::GDTDescriptor::set_access(uint8_t access)
{
    this->access = access;
}

uint8_t boot::GDTDescriptor::get_access()
{
    return access;
}

void boot::GDTDescriptor::set_flags(uint8_t flags)
{
    this->flags |= flags & 0xf0;
}

uint8_t boot::GDTDescriptor::get_flags()
{
    return flags & 0xf0;
}

boot::GDTDescriptor &boot::GDTDescriptor::operator=(boot::GDTDescriptor &other)
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

void boot::GDT::set_descriptor(uint32_t idx, boot::GDTDescriptor *gdt_desc)
{
    if (idx >= GDT_MAX_DESCRIPTORS) // checks index
    {
        return;
    }
    _gdt[idx] = *gdt_desc;
}

boot::GDTDescriptor *boot::GDT::get_descriptor(uint32_t idx)
{
    if (idx >= GDT_MAX_DESCRIPTORS) // checks index
    {
        return 0;
    }
    return &_gdt[idx];
}

void boot::GDT::flush()
{
    gdt_reg.limit = sizeof(_gdt) - 1;
    gdt_reg.base = (uint32_t)_gdt;

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
                 : "m"(this->gdt_reg),
                   "i"((uint32_t)boot::KERNEL_DATA_SEGMENT),
                   "i"((uint32_t)boot::KERNEL_CODE_SEGMENT));
}

void boot::GDT::setup()
{
    /**
     * As required by x86 processors, set first descriptor as NULL 
     * descriptor.
     */
    _gdt[0].set_base(0);
    _gdt[0].set_limit(0);
    _gdt[0].set_access(0);
    _gdt[0].set_flags(0);

    /**
     * Setting default code descriptor.
     */
    _gdt[1].set_base(0);
    _gdt[1].set_limit(0xffffffff);
    _gdt[1].set_access(GDT_DESC_ACCESS_CD_SEG | GDT_DESC_ACCESS_EXEC | GDT_DESC_ACCESS_RW | GDT_DESC_ACCESS_P);
    _gdt[1].set_flags(GDT_DESC_FLAG_GR | GDT_DESC_FLAG_SZ);

    /**
     * Setting default data descriptor.
     */
    _gdt[2].set_base(0);
    _gdt[2].set_limit(0xffffffff);
    _gdt[2].set_access(GDT_DESC_ACCESS_CD_SEG | GDT_DESC_ACCESS_RW | GDT_DESC_ACCESS_P);
    _gdt[2].set_flags(GDT_DESC_FLAG_GR | GDT_DESC_FLAG_SZ);

    flush();
}
