/**
 * Headder contains multiboot information structure specified by 
 * the multiboot specification.
 */

#ifndef ARCH_I386_MULTIBOOT_HPP
#define ARCH_I386_MULTIBOOT_HPP

#include <stdint.h>

namespace boot
{
/** 
 * Multiboot info structure passed from boot loader.
 */
struct MultibootInfo
{

    uint32_t flags;        /**< Various flags (see multiboot specification) */
    uint32_t memory_lower; /**< Memory in KB starting from 0x0 */
    uint32_t memory_upper; /**< Memory in KB starting from 0x100000 */
    uint32_t boot_device;  /**< Device that contains the kernel image */
    uint32_t cmdline;      /**< Command line argument. */
    uint32_t mods_count;   /**< Number of loaded modules */
    uint32_t mods_addr;    /**< Module address array address */
    uint32_t syms0;
    uint32_t syms1;
    uint32_t syms2;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t bootloader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint32_t vbe_interface_addr;
    uint16_t vbe_interface_len;
};
} // namespace boot

#endif /* ARCH_I386_MULTIBOOT_HPP */
