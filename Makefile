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
#EMU=bochs

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
export CPPFLAGS:=

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

.PHONEY: build clean run

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
	rm -rf ${BUILD_DIR}

# Install grub
grub: build
	mkdir -p ${SYSROOT}/boot/grub
	cp bootloader/grub/grub.cfg ${SYSROOT}/boot/grub/grub.cfg

# Create OS grub ISO image
grub-iso: grub
	mkdir -p ${OS_IMG_DIR}
	/usr/local/Cellar/i386-elf-grub/2.04/bin/i386-elf-grub-mkrescue -o ${OS_IMG_DIR}/myos.iso ${SYSROOT}

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
run: ${EMU}-run-${OS_IMG_FORMAT}
