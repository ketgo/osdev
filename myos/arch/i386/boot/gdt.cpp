#include <stdint.h>

#include <boot/gdt.h>

void boot::GDTDescriptor::set_base(uint32_t base)
{
    this->base_lo = 0;
    this->base_lo = uint16_t(base & 0xffff);

    this->base_mid = 0;
    this->base_mid = uint8_t((base >> 16) & 0xff);

    this->base_hi = 0;
    this->base_hi = uint8_t((base >> 24) & 0xff);
}

uint32_t boot::GDTDescriptor::get_base()
{
    uint32_t base;

    base = uint32_t(this->base_lo);
    base += uint32_t(this->base_mid) << 16;
    base += uint32_t(this->base_hi) << 24;

    return base;
}

void boot::GDTDescriptor::set_limit(uint32_t limit)
{
    this->limit_lo = uint16_t(limit & 0xffff);
    this->flags |= uint8_t((limit >> 16) & 0x0f);
}

uint32_t boot::GDTDescriptor::get_limit()
{
    uint32_t limit;

    limit = uint32_t(this->limit_lo);
    limit += uint32_t(this->flags & 0x0f) << 16;

    return limit;
}

void boot::GDTDescriptor::set_access(uint8_t access)
{
    this->access = access;
}

uint8_t boot::GDTDescriptor::get_access()
{
    return this->access;
}

void boot::GDTDescriptor::set_flags(uint8_t flags)
{
    this->flags |= flags & 0xf0;
}

uint8_t boot::GDTDescriptor::get_flags()
{
    return this->flags & 0xf0;
}

boot::GDTDescriptor &boot::GDTDescriptor::operator=(boot::GDTDescriptor &other)
{
    if (this != &other) // self-assignment check
    {
        this->set_base(other.get_base());
        this->set_limit(other.get_limit());
        this->set_access(other.get_access());
        this->set_flags(other.get_flags());
    }
    return *this;
}

/**
 * This struct describes a GDT pointer. It points to the start of our array of
 * GDT entries, and is in the format required by the lgdt instruction.
 */
struct __attribute__((packed)) GDTRegister
{
    uint16_t limit; /**< Size of gdt table minus one. */
    uint32_t base;  /**< The base table address. */
};

boot::GDT boot::gdt;

void boot::GDT::set_descriptor(uint32_t idx, boot::GDTDescriptor *gdt_desc)
{
    if (idx >= GDT_MAX_DESCRIPTORS) // checks index
    {
        return;
    }
    this->_gdt[idx] = *gdt_desc;
}

boot::GDTDescriptor *boot::GDT::get_descriptor(uint32_t idx)
{
    if (idx >= GDT_MAX_DESCRIPTORS) // checks index
    {
        return 0;
    }
    return &this->_gdt[idx];
}

void boot::GDT::flush()
{
    /* Xen HVM incorrectly stores a pointer to the gdt_ptr, instead
	   of the gdt_ptr contents.  Thus, make it static so it will
	   stay in memory, at least long enough that we switch to the
	   proper kernel GDT. 
       
       see: https://github.com/torvalds/linux/blob/0adb32858b0bddf4ada5f364a84ed60b196dbcda/arch/x86/boot/pm.c#L84 
    */
    static GDTRegister gdt_reg;

    gdt_reg.limit = sizeof(this->_gdt) - 1;
    gdt_reg.base = (uint32_t)this->_gdt;

    asm volatile("lgdtl   %0\n\t" ::"m"(gdt_reg));
}

void boot::GDT::initialize()
{
}
