.EXPORT_ALL_VARIABLES:
BINUTILS_VERSION=2.28
GCC_VERSION=7.1.0
GDB_VERSION=8.0
BUILD_DIR=/opt/cross/build-${TARGET}
TARGET=i686-elf
PREFIX=${BUILD_DIR}/output
BIN_PATH=${PREFIX}/bin

# Setup development enviornment 
.PHONY: setup
setup:
	./setup.sh build binutils gcc gdb
	echo -e "Installing NASM..."
	sudo apt-get install nasm -y
	echo -e "Installing Qemu..."
	sudo apt-get install qemu -y

# Remove development enviornment
.PHONY: remove
remove:
	echo -e "Removing cross compiler ans binutils..."
	./setup.sh remove
	echo -e "Removing NASM..."
	sudo apt-get remove nasm -y
	echo -e "Remove Qemu..."
	sudo apt-get remove qemu -y

# Build kernel
.PHONY: build
build:
	mkdir -p build/${TARGET}
	nasm -felf32 kernel/main.asm -o build/${TARGET}/main.o
	${BIN_PATH}/i686-elf-g++ -c kernel/kernel.cpp -o build/${TARGET}/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	${BIN_PATH}/i686-elf-gcc -T kernel/linker.ld -o build/${TARGET}/myos.bin -ffreestanding -O2 -nostdlib build/${TARGET}/main.o build/${TARGET}/kernel.o -lgcc
	rm build/${TARGET}/*.o

# Run kernel
.PHONY: run
run: clean build
	qemu-system-x86_64 -kernel build/${TARGET}/myos.bin

# Cleanup
.PHONY: clean
clean:
	rm -r build
