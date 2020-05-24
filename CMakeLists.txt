CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

project(osdev)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/myos)
