/**
 * Global Descriptor Table
 * =======================
 *
 * The Global Descriptor Table (GDT) is a data structure used by Intel x86
 * processors starting with 80286 to define characteristics of the various
 * memory areas used during program execution. These memory areas are called
 * segments in Intel terminology.
 * GDT can hold things other than segment descriptors as well. Every 8-byte
 * entry in the GDT is a descriptor, these can be Task State Segment (TSS)
 * descriptors, Local Descriptor table (LDT) descriptors, or Call Gate desc.
 *
 * On most modern Operating Systems segments are initialized to cover the
 * entire addressable memory, which make segment-relative addressing
 * transparent to the user.
 * Also note that to implement memory protection, segmentation has been
 * replaced by paging.
 *
 * To reference a segment, a program must use its index inside the GDT.
 * Such an index is called a *segment selector*. The selector must be loaded
 * into a *segment register* to be used.
 * Every machine instruction referencing memory has an implicit segment
 * register. The segment register can be explicitly overwritten by adding a
 * segment prefix before the instruction.
 *
 * Loading a selector into a segment register automatically reads the GDT
 * and stores properties within the processor itself. Subsequent modifications
 * of the GDT will not be effective unless the segment reg is reloaded.
 *
 *  63          56  55  52 51        48 47           40 39            32
 * +--------------+-------+------------+---------------+----------------+
 * | Base (24:31) | Flags | Lim (16:19)| Access Byte   |  Base (16:23)  |
 * +--------------+-------+------------+---------------+----------------+
 *  31                               16 15                             0
 * +-----------------------------------+--------------------------------+
 * |            Base (0:15)            |         Limit (0:15)           |
 * +-----------------------------------+--------------------------------+
 *
 * Note that modern operating systems, uses paging as memory protection
 * mechanism. GDT is set up to use flat seg
 *
 * Access byte
 * -----------
 *
 *   7                             0
 * +---+---+---+---+---+----+----+---+
 * | P |  DPL  | S | E | DC | RW | A |
 * +---+---+---+---+---+----+----+---+
 *
 * P: Present bit.
 *  This must be 1 for all valid selectors.
 * DPL: Privilege.
 *  Contains the ring level, 0 = highest, 3 = lowest.
 * S: Segment Type Bit.
 *  If 1 segment is for Code/Data else its for system (eg. a Task State Segment).
 * E: Executable bit.
 *  If 1 code in this segment can be executed.
 * DC: Direction/Conforming bit.
 *  For data selectors:  tells the direction. 0 the segment grows up. 1 the
 *  segment grows down, i.e. the offset has to be greater than the limit
 *  For code selectors: if 1 code in this segment can be executed from an
 *  equal or lower privilege level. The DPL-bits represent the highest 
 *  privilege level that is allowed to execute the segment.
 *  Note that the privilege level remains the same, i.e. a far-jump form ring 
 *  3 to a dpl 2 segment remains in ring 3 after the jump.
 *  If 0 code in this segment can only be executed from the ring set in dpl.
 * RW: Readable/Writable bit.
 *  Readable bit for code selectors: Whether read access for this segment is
 *  allowed. Write access is never allowed for code segments.
 *  Writable bit for data selectors: Whether write access for this segment is
 *  allowed. Read access is always allowed for data segments.
 * A: Accessed bit.
 *  The CPU sets this to 1 when the segment is accessed.
 *
 * Flags
 * -----
 *
 *   7                                   0
 * +----+----+----+-----+----+----+----+----+
 * | GR | SZ |    | AVL |     Limit High    |
 * +----+----+----+-----+----+----+----+----+
 *
 * GR: Granularity bit.
 *  If 0 the limit is in 1B blocks (byte granularity).
 *  If 1 the limit is in 4KB blocks (page granularity).
 * SZ: Size bit.
 *  If 0 the selector defines 16 bit protected mode.
 *  If 1 it defines 32 bit protected mode. You can have both 16 bit and 32 bit
 *  selectors at once.
 * AVL: Bit available for systems programers.
 */

#ifndef ARCH_I386_GDT_HPP
#define ARCH_I386_GDT_HPP

#include <stdint.h>

namespace I386
{

/**
 * Maximum number of descriptors in GDT. The first descriptor is always
 * Null descriptor in compliance with x86 processors.
 */
#define GDT_MAX_DESCRIPTORS 3

/**
 * Enumerated list of different segment selectors.
 */
enum Selectors
{
    /** 
    * Kernel code segment is set at an offset of 8 bytes, which is 
    * 0x08 in hex, from the base address of GDT. This is set in 
    * the initialize() method of the GDT class.
    */
    KERNEL_CODE_SEGMENT = 0X08,
    /** 
    * Kernel data segment is set at an offset of 16 bytes, which is 
    * 0x10 in hex, from the base address of GDT. This is set in the 
    * initialize() method. of the GDT class.
    */
    KERNEL_DATA_SEGMENT = 0X10
};

/**
 * Enumerated list of GDT descriptor access bits
 */

/**
 * Sets Readable/Writable bit. [Default unset]
 * 
 * Readable bit for code selectors: Whether read access for this segment is
 * allowed. Write access is never allowed for code segments.
 * 
 * Writable bit for data selectors: Whether write access for this segment is
 * allowed. Read access is always allowed for data segments.
 */
#define GDT_DESC_ACCESS_RW 0x02

/**
 * Sets Direction/Conforming bit. [Default unset]
 * 
 * For data selectors:  tells the direction. 0 the segment grows up. 1 the
 * segment grows down, i.e. the offset has to be greater than the limit
 * 
 * For code selectors: if 1 code in this segment can be executed from an
 * equal or lower privilege level. The DPL-bits represent the highest
 * privilege level that is allowed to execute the segment. Note that the 
 * privilege level remains the same, i.e. a far-jump form ring 3 to a DPL 
 * 2 segment remains in ring 3 after the jump. If 0 code in this segment 
 * can only be executed from the ring set in DPL.
 */
#define GDT_DESC_ACCESS_DC 0x04

/**
 * Sets Executable bit. [Default unset]
 * 
 * If 1 code in this segment can be executed.
 */
#define GDT_DESC_ACCESS_EXEC 0x08

/**
 * Sets Segment Type Bit. [Default unset]
 * 
 * If 1 segment is for Code/Data else its for system (eg. a Task State Segment).
 */
#define GDT_DESC_ACCESS_CD_SEG 0x10

/**
 * Sets Privilege Bits. [Default unset for ring 0]
 * 
 * Contains the ring level, 0 = highest, 3 = lowest.
 */
#define GDT_DESC_ACCESS_DPL_RING_1 0x20
#define GDT_DESC_ACCESS_DPL_RING_2 0x40
#define GDT_DESC_ACCESS_DPL_RING_3 0x60

/**
 * Present bit.
 *  
 * This must be 1 for all valid selectors.
 */
#define GDT_DESC_ACCESS_P 0x80

/**
 * Enumerated list of GDT descriptor flag bits.
 */

/**
 * Sets Granularity bit.
 * 
 * If 0 the limit is in 1B blocks (byte granularity).
 * If 1 the limit is in 4KB blocks (page granularity).
 */
#define GDT_DESC_FLAG_GR 0x80

/** 
 * Sets Size bit.
 * 
 * If 0 the selector defines 16 bit protected mode.
 * If 1 it defines 32 bit protected mode. You can have both 
 *  16 bit and 32 bit selectors at once.
 */
#define GDT_DESC_FLAG_SZ 0x40

/**
 * Sets systems programers bit.
 */
#define GDT_DESC_FLAG_AVL 0x10

/**
 * GDT descriptor class.
 * 
 * NOTE: Please do not add any virtual methods in the class definition. Doing so will 
 * change the memory layout causing undefined behaviour and most likely triple fault. 
 * Only plain membor methods should be added.
 */
class __attribute__((packed)) GDTDescriptor
{
private:
    uint16_t limit_lo; /**< The lower 16 bits of the limit (15..0). */
    uint16_t base_lo;  /**< The lowers 16 bits of the base (15..0). */
    uint8_t base_mid;  /**< The next 8 bits of the base (23..16). */
    uint8_t access;    /**< Pr, DPL, S, Ex, DC, RW, Ac. */
    uint8_t flags;     /**< Gr, Sz, Limit High (19..16). */
    uint8_t base_hi;   /**< The last 8 bits of the base (31..24). */

public:
    /**
     * Default constructor.
     */
    GDTDescriptor(){};

    /**
     * Initialization constructor.
     * 
     * @param base starting address of memory segment
     * @param limit ending address of memory segment
     * @param access access properties of memory segment
     * @param flags flags of memory segment
     */
    GDTDescriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

    /**
     * Set memory segment starting address
     * 
     * @param base starting address
     */
    void set_base(uint32_t base);
    uint32_t get_base();

    /**
     * Set memory segment ending address
     * 
     * @param limit ending address
     */
    void set_limit(uint32_t limit);
    uint32_t get_limit();

    /**
     * Set memory segment access properties
     * 
     * @param access access properties
     */
    void set_access(uint8_t access);
    uint8_t get_access();

    /**
     * Set memory segment flags
     * 
     * @param flags segment flags
     */
    void set_flags(uint8_t flags);
    uint8_t get_flags();

    /**
     * Descriptor assignment operator
     */
    GDTDescriptor &operator=(GDTDescriptor &other);
};

/**
 * This struct describes a GDT pointer. It points to the start of our array of
 * GDT entries, and is in the format required by the lgdt instruction.
 */
struct __attribute__((packed)) GDTRegister
{
    uint16_t limit; /**< Size of gdt table minus one. */
    uint32_t base;  /**< The base table address. */
};

/**
 * The GDT class containing an array of descriptors.
 */
class GDT
{
private:
    GDTRegister gdt_reg;                     /**< GDT register to load descriptors */
    GDTDescriptor _gdt[GDT_MAX_DESCRIPTORS]; /**< Array of global descriptors. */

public:
    /**
    * Setup GDT with default descriptors.
    */
    void setup();

    /**
     * Install the set descriptors in GDT using the `lgdt` instruction.
     */
    void flush();

    /**
     * Set descriptor in GDT.
     * 
     * @param idx index position of the descriptor in table
     * @param gdt_desc pointer to GDT descriptor
     */
    void set_descriptor(uint32_t idx, GDTDescriptor *gdt_desc);

    /**
     * Get descriptor in GDT.
     * 
     * @param idx index postion of the descriptor to get
     * @returns pointer to descriptor
     */
    const GDTDescriptor *get_descriptor(uint32_t idx);
};

/**
 * GDT for kernel boot sequence
 */
extern GDT gdt;

} // namespace I386

#endif /* ARCH_I386_GDT_HPP */