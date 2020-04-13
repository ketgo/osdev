#include <boot/console.h>
#include <boot/gdt.h>
#include <boot/pm.h>

void boot::start_protected_mode()
{
    boot::console.printf("Setting up GDT...\n");
    boot::gdt.initialize();
}