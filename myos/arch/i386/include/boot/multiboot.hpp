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

    uint32_t m_flags;
    uint64_t m_memorySize;

    //	uint32_t	m_memoryLo;
    //	uint32_t	m_memoryHi;
    uint32_t m_bootDevice;
    uint32_t m_cmdLine;
    uint32_t m_modsCount;
    uint32_t m_modsAddr;
    uint32_t m_syms0;
    uint32_t m_syms1;
    uint32_t m_syms2;
    uint32_t m_mmap_length;
    uint32_t m_mmap_addr;
    uint32_t m_drives_length;
    uint32_t m_drives_addr;
    uint32_t m_config_table;
    uint32_t m_bootloader_name;
    uint32_t m_apm_table;
    uint32_t m_vbe_control_info;
    uint32_t m_vbe_mode_info;
    uint16_t m_vbe_mode;
    uint32_t m_vbe_interface_addr;
    uint16_t m_vbe_interface_len;
};
} // namespace boot

#endif /* ARCH_I386_MULTIBOOT_HPP */
