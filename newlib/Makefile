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
#	Libc Makefile
#
#	- Ketan Goyal
# ----------------------

# Targeted CPU arch
TARGET?=

# Host
HOST?=

# Default build directory
BUILD_DIR?=buid

# Installation destination
DESTDIR?=

# Libc build directory
LIBC_BUILD_DIR:=${BUILD_DIR}/libc

.PHONY: install-headers install clean

install:
	cd ${LIBC_BUILD_DIR} && \
	AR="" AS="" CC="" CPP="" ${CURDIR}/configure --prefix=/usr --target=${HOST} && \
	${MAKE} && \
	${MAKE} DESTDIR=${DESTDIR} install;
	cp -apR ${DESTDIR}/usr/${HOST}/* ${DESTDIR}/usr/;
	rm -rf ${DESTDIR}/usr/${HOST}

install-headers:
	mkdir -p ${LIBC_BUILD_DIR}

clean:
	rm -rf ${LIBC_BUILD_DIR}
