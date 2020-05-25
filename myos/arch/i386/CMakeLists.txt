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

#--------------------------------------
# TODO: Move all this to toolchain file
#--------------------------------------

# Ctor start files
set(CRTI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/crti.S PARENT_SCOPE)
set(CRTN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/crtn.S PARENT_SCOPE)
execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} ${CFLAGS} ${LDFLAGS} -print-file-name=crtbegin.o
    OUTPUT_VARIABLE CRTBEGIN_SOURCES
)
execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} ${CFLAGS} ${LDFLAGS} -print-file-name=crtend.o
    OUTPUT_VARIABLE CRTEND_SOURCES
)
set_source_files_properties(
    ${CRTBEING_SOURCES} ${CRTEND_SOURCES}
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
)

#--------------------------------------

# Linker script file
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld PARENT_SCOPE)

# Set arch specific compiler flags
set(KERNEL_ARCH_C_FLAGS "-mno-red-zone")
set(KERNEL_ARCH_CXX_FLAGS "-mno-red-zone")
