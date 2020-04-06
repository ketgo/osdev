# Targeted CPU arch
TARGET=i386

# Source code directoy
SRC_DIR=./kernel

# Arcgitecture specific source code directoy
ARCH_DIR=${SRC_DIR}/arch/${TARGET}

# Base build directoy
BUILD_DIR=./build/${TARGET}

# Kernel build directoy
KERNEL_BUILD_DIR=${BUILD_DIR}/kernel

# OS system-root build directory
SYSROOT_BUILD_DIR=${BUILD_DIR}/sysroot

# OS image build directoy
IMG_BUILD_DIR=${BUILD_DIR}/img
IMG_FORMAT=iso

# Bootloader to use
BOOTLOADER=grub

# Emulator to use
EMU=qemu

# Assembler
ASM=nasm
# Assembler flags
AFLAG=-felf32

# C++ cross-complier
CPP=${TARGET}-elf-g++
# C++ complier flags
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -I${SRC_DIR}/include -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# C cross-compier
CC=${TARGET}-elf-gcc
# C compiler flags
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -O2
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc

# Build kernel
.PHONY: build
build:
	mkdir -p ${KERNEL_BUILD_DIR}
	${ASM} ${AFLAG} ${ARCH_DIR}/boot.S -o ${KERNEL_BUILD_DIR}/main.o
	${CPP} -c ${SRC_DIR}/kernel/kernel.cpp -o ${KERNEL_BUILD_DIR}/kernel.o ${CPPFLAGS}
	${CC} -T ${ARCH_DIR}/linker.ld -o ${KERNEL_BUILD_DIR}/myos.bin ${CFLAGS} ${LIBS} ${LDFLAGS} ${KERNEL_BUILD_DIR}/*.o
	rm ${KERNEL_BUILD_DIR}/*.o

# Build system root
.PHONY: sysroot
sysroot: build
	mkdir -p ${SYSROOT_BUILD_DIR}/boot
	cp ${KERNEL_BUILD_DIR}/myos.bin	${SYSROOT_BUILD_DIR}/boot/myos.bin

# Install grub
.PHONY: grub
grub: sysroot
	mkdir -p ${SYSROOT_BUILD_DIR}/boot/grub
	cp bootloader/grub/grub.cfg ${SYSROOT_BUILD_DIR}/boot/grub/grub.cfg

# Create OS grub ISO image
.PHONY: grub-iso
grub-iso: grub
	mkdir -p ${IMG_BUILD_DIR}
	/usr/local/Cellar/i386-elf-grub/2.04/bin/i386-elf-grub-mkrescue -o ${IMG_BUILD_DIR}/myos.iso ${SYSROOT_BUILD_DIR}

# Run kernel binaries on QEMU
.PHONY: qemu-run-bin
qemu-run-bin: clean build
	qemu-system-x86_64 -kernel ${KERNEL_BUILD_DIR}/myos.bin

# Run OS image on QEMU
.PHONY: qemu-run-iso
qemu-run-iso: clean ${BOOTLOADER}-iso
	qemu-system-x86_64 -readconfig qemurc

# Run kernel binaries on Bochs
.PHONY: bochs-run-bin
bochs-run-bin: clean build
	echo "ERROR: Kernel binary run not supported by Bochs"
	exit 1

# Run kernel binaries on Bochs
.PHONY: bochs-run-iso
bochs-run-iso: clean ${BOOTLOADER}-iso
	bochs -f bochsrc -q

# Run OS
.PHONY: run
run: ${EMU}-run-${IMG_FORMAT}

# Cleanup
.PHONY: clean
clean:
	rm -r build
