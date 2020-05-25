CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

project(osdev)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bootloaders/grub)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/myos)

#---
# TODO: Arch specific variables should be set in toolchain file
#---

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qemurc ${CMAKE_CURRENT_BINARY_DIR}/qemurc)
add_custom_target(
    run-qemu
    COMMAND ${CMAKE_SYSTEM_PROCESSOR}-elf-grub-mkrescue -o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/myos.iso ${CMAKE_SYSROOT}
    COMMAND qemu-system-${CMAKE_SYSTEM_PROCESSOR} -readconfig ${CMAKE_CURRENT_BINARY_DIR}/qemurc
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bochsrc ${CMAKE_CURRENT_BINARY_DIR}/bochsrc)
add_custom_target(
    run-bochs
    COMMAND ${CMAKE_SYSTEM_PROCESSOR}-elf-grub-mkrescue -o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/myos.iso ${CMAKE_SYSROOT}
    COMMAND bochs -f ${CMAKE_CURRENT_BINARY_DIR}/bochsrc -q
)

#---
