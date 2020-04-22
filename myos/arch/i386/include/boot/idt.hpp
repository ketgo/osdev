/**
 * Interrupt Descriptor Table
 * ==========================
 *
 * The Interrupt Descriptor Table (IDT) is data structure used by Intel x86
 * processors to implement an interrupt vector table.
 * The IDT is used by the processor to determine the correct response to
 * interrupts and exceptions.
 * In protected mode, the IDT is an array of 256 8-byte descriptors stored
 * consecutively in memory and indexed by an interrupt vector.
 * The descriptors contains memory addresses and a segment selector of code
 * to execute when the relative event happens. The segment selector must
 * point to a valid GDT entry. The code to execute is referred as interrupt
 * service routine (ISR).
 *
 *  63                              48 47            40 39             32
 * +----------------------------------+----------------+-----------------+
 * |            Base (16:31)          |      Flags     |      Zero       |
 * +----------------------------------+----------------+-----------------+
 *  31                              16 15                              0
 * +----------------------------------+---------------------------------+
 * |          Selector (0:15)         |           Base (0:15)           |
 * +----------------------------------+---------------------------------+
 *
 * Flags
 * -----
 *
 *    7
 *  +---+-----+---+-----------+
 *  | P | DPL | S |     T     |
 *  +---+-----+---+-----------+
 *
 * P: Present.
 *  Set to 0 for unused interrupts
 * DPL: Privilege.
 *  Specifies the minimum privilege level for the calling descriptor.
 * S: Storage Segment.
 *  Set to 0 for interrupt gates
 * T: Gate Type.
 *  0x0E for 32-bit interrupt gate
 *  0x0F for 32-bit trap gate
 *
 *
 * The first 32 IDT elements are reserved for processor exceptions.
 *
 * | NUM | Description                  |
 * |-----|------------------------------|
 * |   0 | Division by zero             |
 * |   1 | Debugger                     |
 * |   2 | Non maskable interrupt       |
 * |   3 | Breakpoint                   |
 * |   4 | Overflow                     |
 * |   5 | Bounds                       |
 * |   6 | Invalid opcode               |
 * |   7 | Coprocessor not available    |
 * |   8 | Double fault                 |
 * |   9 | Coprocessor segment overrun  |
 * |  10 | Invalid Task State Segment   |
 * |  11 | Segment not present          |
 * |  12 | Stack Fault                  |
 * |  13 | General protection fault     |
 * |  14 | Page fault                   |
 * |  15 | reserved                     |
 * |  16 | Math Fault                   |
 * |  17 | Alignment Check              |
 * |  18 | Machine Check                |
 * |  19 | SIMD Floating-Point Exception|
 * |  20 | Virtualization Exception     |
 * |  21 | Control Protection Exception |
 *
 * Entries 22 to 31 are reserved by Intel.
 */

#ifndef ARCH_I386_IDT_HPP
#define ARCH_I386_IDT_HPP

#include <stdint.h>

#include <boot/isr.hpp>


namespace boot
{

/**
 * Maximum number of descriptors in IDT.
 */
#define IDT_MAX_DESCRIPTORS 256

/**
 * Gate type bits.
 * 
 * 32-bit interrupt gate
 */
#define IDT_DESC_FLAG_INT_BIT32 0x0E

/**
 * Gate type bits.
 * 
 * 32-bit trap gate
 */
#define IDT_DESC_FLAG_TRAP_BIT32 0x0F

/**
 * Sets Privilege Bits. [Default unset for ring 0]
 * 
 * Contains the ring level, 0 = highest, 3 = lowest.
 */
#define IDT_DESC_FLAG_DPL_RING_1 0x20
#define IDT_DESC_FLAG_DPL_RING_2 0x40
#define IDT_DESC_FLAG_DPL_RING_3 0x60

/**
 * Present bit. [Default unset]
 * 
 * Set to 0 for unused interrupts
 */
#define IDT_DESC_FLAG_PRESENT 0x80

/** 
 * Interrupt descriptor class.
 * 
 * NOTE: Please do not add any virtual methods in the class definition. Doing so will 
 * change the memory layout causing undefined behaviour and most likely triple fault. 
 * Only plain membor methods should be added. 
 */
class __attribute__((packed)) IDTDescriptor
{
private:
    uint16_t offset_lo;     /**< The lower 16 bits of the IRQ handler address to jump to. */
    uint16_t selector;      /**< Code segment selector in GDT. */
    const uint8_t zero = 0; /**< This must always be zero. */
    uint8_t flags;          /**< More flags. See documentation. */
    uint16_t offset_hi;     /**< The upper 16 bits of the IRQ handler address to jump to. */
public:
    /**
     * Default constructor.
     */
    IDTDescriptor(){};

    /**
     * Initialization constructor.
     * 
     * @param isr_handler pointer to ISR
     * @param selector code selector
     * @param flags descriptor flags to set
     */
    IDTDescriptor(uint32_t isr_handler, uint16_t selector, uint8_t flags);

    /**
     * Set interrupt service routine (ISR).
     * 
     * @param handler pointer to ISR
     */
    void set_handler(uint32_t isr_handler);
    uint32_t get_handler();

    /**
     * Set code segment selector configured in GDT.
     * 
     * @param selector code selector
     */
    void set_selector(uint16_t selector);
    uint16_t get_selector();

    /**
     * Set interrupt descriptor flags
     * 
     * @param flags descriptor flags to set
     */
    void set_flags(uint8_t flags);
    uint8_t get_flags();

    /**
     * Descriptor assignment operator
     */
    IDTDescriptor &operator=(IDTDescriptor &other);
};

/**
 * A struct describing a pointer to an array of interrupt handlers.
 * This is in a format suitable for giving to 'lidt'.
 */
struct __attribute__((packed)) IDTRegister
{
    uint16_t limit; /**< Size of idt table minus one. */
    uint32_t base;  /**< The first entry address. */
};

/**
 * The IDT class containing an array of descriptors.
 */
class IDT
{
private:
    IDTRegister idt_reg;                     /**< IDT register used by x86 arch processors to load Interrupt vectors */
    IDTDescriptor _idt[IDT_MAX_DESCRIPTORS]; /**< Array of descriptors. */

public:
    /**
    * Initialize the IDT with default descriptors. 
    */
    void initialize();

    /**
     * Install the set descriptors in IDT using the `lidt` instruction.
     */
    void flush();

    /**
     * Set descriptor in IDT.
     * 
     * @param n interrupt number to be used for IDT descriptor. This is the 
     *  index position of the descriptor in IDT.
     * @param idt_desc pointer to IDT descriptor.
     */
    void set_descriptor(uint32_t n, IDTDescriptor *idt_desc);

    /**
     * Get descriptor in IDT.
     * 
     * @param n interrupt number used for the IDT descriptor. This is the 
     *  index postion of the descriptor in IDT.
     * @returns pointer to IDT descriptor.
     */
    IDTDescriptor *get_descriptor(uint32_t n);
};

/**
 * IDT for kernel boot sequence
 */
extern IDT idt;

} // namespace boot

#endif /* ARCH_I386_IDT_HPP */
