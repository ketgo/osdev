/**
 * Header file containing Interrupt Service Request (ISR) stack frame structure 
 * and known ISR number.
 */

#ifndef ARCH_ISR_HPP
#define ARCH_ISR_HPP

#include <stdint.h>

/*
 * Interrupt numbers
 */
#define ISR_DIV_BY_ZERO 0
#define ISR_PAGE_FAULT 14
#define ISR_IRQ0 32
#define ISR_TIMER 32
#define ISR_KEYBOARD 33
#define ISR_SERIAL2 35
#define ISR_SERIAL1 36
#define ISR_SYSCALL 128

namespace kernel
{
    /*
    * Architecture-specific ISR arguments.
    * 
    * Some registers are automatically pushed by the processor while
    * others are pushed by the dispatching code for preservation or
    * for later reference.
    */
    struct __attribute__((packed)) ISRArg
    {
        uint32_t ds;       /* data segment selector */
        uint32_t edi;      /* pushed by pusha */
        uint32_t esi;      /* pushed by pusha */
        uint32_t ebp;      /* pushed by pusha */
        uint32_t esp;      /* pushed by pusha */
        uint32_t ebx;      /* pushed by pusha */
        uint32_t edx;      /* pushed by pusha */
        uint32_t ecx;      /* pushed by pusha */
        uint32_t eax;      /* pushed by pusha */
        uint32_t err_code; /* error code pushed by the processor on exception */
        uint32_t eip;      /* pushed by the processor */
        uint32_t cs;       /* pushed by the processor */
        uint32_t eflags;   /* pushed by the processor */
        uint32_t usr_esp;  /* pushed by the processor */
        uint32_t ss;       /* pushed by the processor */
    };

} // namespace kernel

#endif /* ARCH_ISR_HPP */
