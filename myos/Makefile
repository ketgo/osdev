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

# ----------------------
#	Kernel Makefile
#
#	- Ketan Goyal
# ----------------------

# Targeted CPU arch
TARGET?=

# Default C and C++ compiler and linker flags
CFLAGS?=-O2 -g
CPPFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=

# Installation directory
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

# Kernel specific source code directory
KERNEL_DIR=kernel

# Architecture specific source code directory
ARCHDIR=arch/${TARGET}

# Device drivers source code directoy
DRIVERDIR=drivers

# NASM assembler
ASM=nasm
# NASM assembler flags
AFLAG=-felf32

# C++ complier flags
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude -I${ARCHDIR}/include -Ilibc/include -fno-rtti
# C compiler flags
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -fno-exceptions
# Linker flags
LDFLAGS:=$(LDFLAGS)
# List of libraries
LIBS:=$(LIBS) -nostdlib -lk

# Include arch specific config
include $(ARCHDIR)/make.config

# Update compiler and linker flags
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

# Include driver config
include $(DRIVERDIR)/make.config

# Update compiler and linker flags
CFLAGS:=$(CFLAGS) $(DRIVER_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(DRIVER_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(DRIVER_LDFLAGS)
LIBS:=$(LIBS) $(DRIVER_LIBS)

# Include kernel specific config
include $(KERNEL_DIR)/make.config

# Kernel compile objects
KERNEL_OBJS:=\
${KERNEL_OBJS} \
${DRIVER_OBJS} \
$(KERNEL_ARCH_OBJS) \

# All compile ojects
OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

# Linking order
LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: myos.kernel

# Build kernel
myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	${CC} -T ${ARCHDIR}/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)

# Get linkers default CRT begin and end object files
$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CPP) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

# Compile NASM assemble code
%.o: %.asm
	${ASM} ${AFLAG} $< -o $@

# Compile MIPS assembly code
%.o: %.S
	$(CPP) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

# Compile all C dependencies
%.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

# Compile all C++ dependencies
%.o: %.cpp
	$(CPP) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f myos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R -p include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
