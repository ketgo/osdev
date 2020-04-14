#include <boot/console.hpp>
#include <boot/gdt.hpp>
#include <boot/pm.hpp>

void boot::start_protected_mode()
{
    boot::console.printf("Setting up GDT...\n");
    boot::gdt.initialize();
}