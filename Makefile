.EXPORT_ALL_VARIABLES:
SRC_DIR=./kernel
SYSROOT_DIR=./sysroot

TARGET=i386
BUILD_DIR=./build/${TARGET}
ARCH_DIR=${SRC_DIR}/arch/${TARGET}

ASM=nasm
AFLAG=-felf32

CPP=${TARGET}-elf-g++
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -I${SRC_DIR}/include -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

CC=${TARGET}-elf-gcc
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -O2
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc

# Build kernel
.PHONY: build
build:
	mkdir -p ${BUILD_DIR}
	${ASM} ${AFLAG} ${ARCH_DIR}/main.asm -o ${BUILD_DIR}/main.o
	${CPP} -c ${SRC_DIR}/kernel/kernel.cpp -o ${BUILD_DIR}/kernel.o ${CPPFLAGS}
	${CC} -T ${ARCH_DIR}/linker.ld -o ${BUILD_DIR}/myos.bin ${CFLAGS} ${LIBS} ${LDFLAGS} ${BUILD_DIR}/main.o ${BUILD_DIR}/kernel.o
	rm ${BUILD_DIR}/*.o

# Run kernel
.PHONY: run
run: clean build
	qemu-system-x86_64 -kernel ${BUILD_DIR}/myos.bin

# Cleanup
.PHONY: clean
clean:
	rm -r build
