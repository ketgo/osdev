CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

project(myos LANGUAGES C CXX ASM ASM_NASM)

# Get kernel generic source files
file(
    GLOB_RECURSE 
    KERNEL_GENERIC_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c
)
# Set kenrel generic compiler flags
set(KERNEL_GENERIC_C_FLAGS "-std=gnu11 -ffreestanding -Wall -Wextra -fno-exceptions")
set(KERNEL_GENERIC_CXX_FLAGS "-ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti")

# Get arch-specific source files to compile
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/arch/${CMAKE_SYSTEM_PROCESSOR})
set_source_files_properties(
    ${CRTBEING_SOURCES} ${CRTEND_SOURCES}
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
)

# Get in-kernel device drivers source files
file(
    GLOB_RECURSE 
    KERNEL_DRIVERS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.c
)

# Set all kernel source files
add_executable(
    myos.kernel     # Target: Kernel image name
    ${KERNEL_GENERIC_SOURCES} 
    ${KERNEL_ARCH_SOURCES} 
    ${KERNEL_DRIVERS_SOURCES} 
)

# Set global header file paths
target_include_directories(
    myos.kernel 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${CMAKE_SYSTEM_PROCESSOR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../libc/include
)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${KERNEL_GENERIC_C_FLAGS} ${KERNEL_ARCH_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KERNEL_GENERIC_CXX_FLAGS} ${KERNEL_ARCH_CXX_FLAGS}")

# Set linker options
target_link_libraries(myos.kernel libk.a)
target_link_options(myos.kernel PRIVATE -nostdlib -lgcc)

install(
    TARGETS myos.kernel 
    RUNTIME DESTINATION ${CMAKE_SYSROOT}/boot
)