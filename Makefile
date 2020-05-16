# Copyright (C) 2020 Ketan Goyal
# 
# This file is part of osdev.
# 
# osdev is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# osdev is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with osdev.  If not, see <http://www.gnu.org/licenses/>.


# ----------------
# 	OS Makefile
#
# 	- Ketan Goyal
# ----------------

#
# TODO: Take the below variables from a make config file
#

export HOST:=i386-elf
export TARGET:=i386

SRC_DIR=${CURDIR}/myos
BUILD_DIR:=${CURDIR}/build/${TARGET}
SYSTEM_HEADER_PROJECTS:=${SRC_DIR}/libc ${SRC_DIR}
PROJECTS:=${SRC_DIR}/libc ${SRC_DIR}

# OS image build directoy
OS_IMG_DIR=${BUILD_DIR}/img
OS_IMG_FORMAT=iso

# Bootloader to use
BOOTLOADER=grub

# Emulator to use
EMU=qemu
# EMU=bochs

export AR:=${HOST}-ar
export AS:=${HOST}-as
export CC:=${HOST}-gcc
export CPP:=${HOST}-g++

export PREFIX:=/usr
export EXEC_PREFIX:=${PREFIX}
export BOOTDIR:=/boot
export LIBDIR:=${EXEC_PREFIX}/lib
export INCLUDEDIR:=${PREFIX}/include

export CFLAGS:=-O2 -g
export CPPFLAGS:=-O2 -g

# Configure the cross-compiler to use the desired system root.
export SYSROOT:=${BUILD_DIR}/sysroot
export CC:=${CC} --sysroot=${SYSROOT}
export CPP:=${CPP} --sysroot=${SYSROOT}

# Work around that the -elf gcc/g++ targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
ifneq (,$(findstring elf,${HOST}))
  export CC:=${CC} -isystem=${INCLUDEDIR}
  export CPP:=${CPP} -isystem=${INCLUDEDIR}
endif

.PHONEY: build clean remove run

#-----------------------------------
# Kernel Build
#-----------------------------------

# Build kernal
build: install-headers
	for PROJECT in ${PROJECTS}; do \
	cd $$PROJECT; DESTDIR="${SYSROOT}" ${MAKE} install; \
	done

# Setup all headers
install-headers:
	for PROJECT in ${SYSTEM_HEADER_PROJECTS}; do \
	cd $$PROJECT; DESTDIR="${SYSROOT}" ${MAKE} install-headers; \
	done

# Cleanup
clean:
	for PROJECT in ${PROJECTS}; do \
	cd $$PROJECT; ${MAKE} clean; \
	done

# Remove
remove: clean
	rm -rf ${BUILD_DIR}

#-------------------------------------------
# Build OS boot image
#------------------------------------------

# Install grub
grub: build
	mkdir -p ${SYSROOT}/boot/grub
	cp bootloaders/grub/grub.cfg ${SYSROOT}/boot/grub/grub.cfg

# Create OS grub ISO image
grub-iso: grub
	mkdir -p ${OS_IMG_DIR}
	/usr/local/Cellar/i386-elf-grub/2.04/bin/i386-elf-grub-mkrescue -o ${OS_IMG_DIR}/myos.iso ${SYSROOT}

#-----------------------------------------
# Run OS
#----------------------------------------

# Run kernel binaries on QEMU
qemu-run-bin: ${KERNEL_BUILD_DIR}/myos.bin
	qemu-system-x86_64 -kernel ${KERNEL_BUILD_DIR}/myos.bin

# Run OS image on QEMU
qemu-run-iso: ${BOOTLOADER}-iso
	qemu-system-x86_64 -readconfig qemurc

# Run kernel binaries on Bochs
bochs-run-bin: ${KERNEL_BUILD_DIR}/myos.bin
	echo "ERROR: Kernel binary run not supported by Bochs"
	exit 1

# Run kernel binaries on Bochs
bochs-run-iso: ${BOOTLOADER}-iso
	bochs -f bochsrc -q

# Run OS
run: clean ${EMU}-run-${OS_IMG_FORMAT}
