/**
 * 8259 Programmable Interrupt Controller (PIC)
 */

#ifndef ARCH_I386_HPP
#define ARCH_I386_HPP

#include <stdint.h>

namespace I386
{
    namespace PIC
    {

// The following devices use PIC 1 to generate interrupts
#define IRQ_TIMER 0
#define IRQ_KEYBOARD 1
#define IRQ_SERIAL2 3
#define IRQ_SERIAL1 4
#define IRQ_PARALLEL2 5
#define IRQ_DISKETTE 6
#define IRQ_PARALLEL1 7

// The following devices use PIC 2 to generate interrupts
#define IRQ_CMOSTIMER 0
#define IRQ_CGARETRACE 1
#define IRQ_AUXILIARY 4
#define IRQ_FPU 5
#define IRQ_HDC 6

        /**
         * Initialize PIC
         * 
         * @param base base address of interrupt vector table
         */
        void initialize(uint16_t base);

        /**
         * Mask interrupt
         * 
         * This function can be used to disable hardware devices.
         * 
         * @param n interrupt number to mask
         */
        void mask(uint8_t n);

        /**
         * Unmask interrupt
         * 
         * This function can be used to enable hardware devices.
         * 
         * @param n interrupt number to unmask
         */
        void unmask(uint8_t n);

    } // namespace PIC

} // namespace I386

#endif /* ARCH_I386_HPP */
