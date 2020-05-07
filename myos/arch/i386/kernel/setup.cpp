#include <i386/idt.hpp>
#include <i386/exception.hpp>

#include <kernel/setup.hpp>
#include <kernel/ioport.hpp>

void kernel::arch_setup()
{
    /* Registering exception and fault interrupt handlers */
    I386::idt.register_isr(0, I386::divide_by_zero_fault);
    I386::idt.register_isr(1, I386::single_step_trap);
    I386::idt.register_isr(2, I386::nmi_trap);
    I386::idt.register_isr(3, I386::breakpoint_trap);
    I386::idt.register_isr(4, I386::overflow_trap);
    I386::idt.register_isr(5, I386::bounds_check_fault);
    I386::idt.register_isr(6, I386::invalid_opcode_fault);
    I386::idt.register_isr(7, I386::no_device_fault);
    I386::idt.register_isr(8, I386::double_fault_abort);
    I386::idt.register_isr(10, I386::invalid_tss_fault);
    I386::idt.register_isr(11, I386::no_segment_fault);
    I386::idt.register_isr(12, I386::stack_fault);
    I386::idt.register_isr(13, I386::general_protection_fault);
    I386::idt.register_isr(14, I386::page_fault);
    I386::idt.register_isr(16, I386::fpu_fault);
    I386::idt.register_isr(17, I386::alignment_check_fault);
    I386::idt.register_isr(18, I386::machine_check_abort);
    I386::idt.register_isr(19, I386::simd_fpu_fault);

    /** Enable interupts */
    kernel::sti();
}
