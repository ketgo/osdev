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

# Get in-kernel device drivers source files
file(
    GLOB_RECURSE 
    KERNEL_DRIVERS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.c
)

add_executable(
    kernel 
    ${CRTI_SOURCES} 
    ${CRTBEGIN_SOURCES} 
    ${KERNEL_GENERIC_SOURCES} 
    ${KERNEL_ARCH_SOURCES} 
    ${KERNEL_DRIVERS_SOURCES} 
    ${CRTBEND_SOURCES}
    ${CRTN_SOURCES}
)

# Add global headers
target_include_directories(
    kernel 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${CMAKE_SYSTEM_PROCESSOR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../libc/include
)

# Set kernel specific compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${KERNEL_GENERIC_C_FLAGS} ${KERNEL_ARCH_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KERNEL_GENERIC_CXX_FLAGS} ${KERNEL_ARCH_CXX_FLAGS}")

# Link library
target_link_libraries(kernel libk.a)
target_link_options(kernel PRIVATE -T ${LINKER_SCRIPT} -nostdlib -lgcc)
