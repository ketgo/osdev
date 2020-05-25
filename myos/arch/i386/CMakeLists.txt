CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

# Get arch-specific source files to compile
file(
    GLOB_RECURSE 
    ARCH_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/*.S
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/*.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.S
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.asm
)
set(KERNEL_ARCH_SOURCES ${ARCH_SOURCES} PARENT_SCOPE)

# Set arch specific compiler flags
set(KERNEL_ARCH_C_FLAGS "-mno-red-zone" PARENT_SCOPE)
set(KERNEL_ARCH_CXX_FLAGS "-mno-red-zone" PARENT_SCOPE)
