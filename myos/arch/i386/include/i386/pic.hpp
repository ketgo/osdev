/**
 * 8259 Programmable Interrupt Controller (PIC)
 */

#ifndef ARCH_I386_HPP
#define ARCH_I386_HPP

#include <stdint.h>

// The following devices use PIC 1 to generate interrupts
#define PIC_IRQ_TIMER 0
#define PIC_IRQ_KEYBOARD 1
#define PIC_IRQ_SERIAL2 3
#define PIC_IRQ_SERIAL1 4
#define PIC_IRQ_PARALLEL2 5
#define PIC_IRQ_DISKETTE 6
#define PIC_IRQ_PARALLEL1 7

// The following devices use PIC 2 to generate interrupts
#define PIC_IRQ_CMOSTIMER 0
#define PIC_IRQ_CGARETRACE 1
#define PIC_IRQ_AUXILIARY 4
#define PIC_IRQ_FPU 5
#define PIC_IRQ_HDC 6

namespace I386
{
    namespace PIC
    {
        /**
         * Initialize and setup PICs
         */
        void setup();

        /**
         * Mask interrupt
         * 
         * This function can be used to disable hardware devices.
         * 
         * @param n interrupt line to mask
         */
        void mask(uint8_t n);

        /**
         * Unmask interrupt
         * 
         * This function can be used to enable hardware devices.
         * 
         * @param n interrupt line to unmask
         */
        void unmask(uint8_t n);

        /**
         * End Of Interrupt (EOI)
         * 
         * This function sends an end of interrupt notification to PICs
         * 
         * @param n interrupt number of the triggered interrupt
         */
        void eoi(uint32_t n);

    } // namespace PIC

} // namespace I386

#endif /* ARCH_I386_HPP */
