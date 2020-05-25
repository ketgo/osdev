CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

include(ExternalProject)
ExternalProject_Add(
    libk
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/configure --prefix=/usr --target=${CMAKE_SYSTEM_PROCESSOR}-elf
    BUILD_COMMAND make
    INSTALL_COMMAND make DESTDIR=${CMAKE_SYSROOT} install;
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SYSROOT}/usr/${CMAKE_SYSTEM_PROCESSOR}-elf ${CMAKE_SYSROOT}/usr
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SYSROOT}/usr/${CMAKE_SYSTEM_PROCESSOR}-elf
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_SYSROOT}/usr/lib/libc.a ${CMAKE_SYSROOT}/usr/lib/libk.a
)
