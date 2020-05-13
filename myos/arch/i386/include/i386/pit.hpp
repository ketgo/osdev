/**
 * 8253 Programmable Interval Timer
 */

#ifndef ARCH_I386_PIT_HPP
#define ARCH_I386_PIT_HPP

namespace I386
{
    namespace PIT
    {
        /**
         * Initialize and setup PIT
         */
        void setup();

        /**
         * Get PIT clock ticks
         * 
         * @returns PIT clock ticks
         */
        uint32_t get_ticks();

    } // namespace PIT

} // namespace I386

#endif /* ARCH_I386_PIT_HPP */
