#include <boot/idt.hpp>

#include <arch/setup.hpp>
#include <arch/io.hpp>
#include <arch/exception.hpp>

void arch::setup()
{
    /* Registering exception and fault interrupt handlers */
    boot::idt.register_isr(0, boot::divide_by_zero_fault);
    boot::idt.register_isr(1, boot::single_step_trap);
    boot::idt.register_isr(2, boot::nmi_trap);
    boot::idt.register_isr(3, boot::breakpoint_trap);
    boot::idt.register_isr(4, boot::overflow_trap);
    boot::idt.register_isr(5, boot::bounds_check_fault);
    boot::idt.register_isr(6, boot::invalid_opcode_fault);
    boot::idt.register_isr(7, boot::no_device_fault);
    boot::idt.register_isr(8, boot::double_fault_abort);
    boot::idt.register_isr(10, boot::invalid_tss_fault);
    boot::idt.register_isr(11, boot::no_segment_fault);
    boot::idt.register_isr(12, boot::stack_fault);
    boot::idt.register_isr(13, boot::general_protection_fault);
    boot::idt.register_isr(14, boot::page_fault);
    boot::idt.register_isr(16, boot::fpu_fault);
    boot::idt.register_isr(17, boot::alignment_check_fault);
    boot::idt.register_isr(18, boot::machine_check_abort);
    boot::idt.register_isr(19, boot::simd_fpu_fault);

    /** Enable interupts */
    boot::sti();
}
