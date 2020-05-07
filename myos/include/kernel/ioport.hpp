/**
 * Basic IO routines. The implementation of these routines are architecture dependent.
 */

#ifndef IOPORT_HPP
#define IOPORT_HPP

#include <stddef.h>
#include <stdint.h>

#include <kernel/defs.hpp>

namespace kernel
{
/**
 * Disable interupts.
 * 
 * This method disables all system interrupts.
 */
void __arch cli();

/**
 * Enable interupts.
 * 
 * This method enables all system interrupts.
 */
void __arch sti();

/**
 * Repeated NOP operation.
 * 
 * Utility method to perform a repeated nop operation.
 */
void __arch rep_nop();

/**
 * Hang CPU.
 * 
 * Utility method to hang CPU.
 */
void __arch __attribute__((noreturn)) hang();

/**
 * Delay I/O
 * 
 * This method can be used to wait a few microseconds for something to complete where 
 * it may be impractical to use full-blown sleep() or delay() timers. It performs a 
 * "dummy" write to a 0x80 I/O address. Reads and writes to 0x80 are basically gauranteed 
 * to not adversely affect the operation of the system, so it is a good choice for such 
 * dummy operations.
 * 
 * ref: https://stackoverflow.com/questions/6793899/what-does-the-0x80-port-address-connect-to
 */
void __arch io_delay(void);

/** 
 * Output byte value on port.
 * 
 * @param  value byte value to output
 * @param port port number
 */
void __arch outb(uint8_t value, uint16_t port);

/**
 * Input byte value from port.
 * 
 * @param port port number
 * @returns byte value at port
 */
uint8_t __arch inb(uint16_t port);

/** 
 * Output 2-byte value on port.
 * 
 * @param  value 2-byte value to output
 * @param port port number
 */
void __arch outw(uint16_t value, uint16_t port);

/**
 * Input 2-byte value from port.
 * 
 * @param port port number
 * @returns 2-byte value at port
 */
uint16_t __arch inw(uint16_t port);

/** 
 * Output 4-byte value on port.
 * 
 * @param  value 4-byte value to output
 * @param port port number
 */
void __arch outl(uint32_t value, uint16_t port);

/**
 * Input 4-byte value from port.
 * 
 * @param port port number
 * @returns 4-byte value at port
 */
uint32_t __arch inl(uint16_t port);

} // namespace kernel

#endif /* IOPORT_HPP */
