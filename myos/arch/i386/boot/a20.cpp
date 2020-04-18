// SPDX-License-Identifier: GPL-2.0-only
/* -*- linux-c -*- ------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007-2008 rPath, Inc. - All Rights Reserved
 *   Copyright 2009 Intel Corporation; author H. Peter Anvin
 *   Copyright 2020; author K. Goyal
 *
 * ----------------------------------------------------------------------- */

/*
 * Enable A20 gate (return -1 on failure)
 */

#include <stdint.h>

#include <boot/console.hpp>
#include <boot/io.hpp>
#include <boot/asm.hpp>
#include <boot/a20.hpp>

#define MAX_8042_LOOPS 100000
#define MAX_8042_FF 32

static int empty_8042(void)
{
    uint8_t status;
    int loops = MAX_8042_LOOPS;
    int ffs = MAX_8042_FF;

    while (loops--)
    {
        boot::io_delay();

        status = boot::inb(0x64);
        if (status == 0xff)
        {
            /* FF is a plausible, but very unlikely status */
            if (!--ffs)
                return -1; /* Assume no KBC present */
        }
        if (status & 1)
        {
            /* Read and discard input data */
            boot::io_delay();
            (void)boot::inb(0x60);
        }
        else if (!(status & 2))
        {
            /* Buffers empty, finished! */
            return 0;
        }
    }

    return -1;
}

/* Returns nonzero if the A20 line is enabled.  The memory address
   used as a test is the int $0x80 vector, which should be safe. */

static int a20_test()
{
    unsigned int *a = (unsigned int *)0x112345;
    unsigned int *b = (unsigned int *)0x012345;
    unsigned int rvalue, saved;

    saved = *a;

    *a = 123;
    if (*a == *b)
    {
        rvalue = 0;
    }
    rvalue = 1;

    *a = saved;

    return rvalue;
}

static void enable_a20_bios(void)
{
    asm volatile("movb $0x2403, %ax\n\t"
                 "int $0x15\n\t");
}

static void enable_a20_kbc(void)
{
    empty_8042();

    boot::outb(0xd1, 0x64); /* Command write */
    empty_8042();

    boot::outb(0xdf, 0x60); /* A20 on */
    empty_8042();

    boot::outb(0xff, 0x64); /* Null command, but UHCI wants it */
    empty_8042();
}

static void enable_a20_fast(void)
{
    uint8_t port_a;

    port_a = boot::inb(0x92); /* Configuration port A */
    port_a |= 0x02;           /* Enable A20 */
    port_a &= ~0x01;          /* Do not reset machine */
    boot::outb(port_a, 0x92);
}

/*
 * Actual routine to enable A20; return 0 on ok, -1 on failure
 */

#define A20_ENABLE_LOOPS 255 /* Number of times to try */

int boot::enable_a20(void)
{
    int loops = A20_ENABLE_LOOPS;
    int kbc_err;

    while (loops--)
    {
        /* First, check to see if A20 is already enabled
		  (legacy free, etc.) */
        if (a20_test())
            return 0;

        /* Next, try the BIOS (INT 0x15, AX=0x2401) */
        enable_a20_bios();
        if (a20_test())
            return 0;

        /* Try enabling A20 through the keyboard controller */
        kbc_err = empty_8042();

        if (a20_test())
            return 0; /* BIOS worked, but with delayed reaction */

        if (!kbc_err)
        {
            enable_a20_kbc();
            if (a20_test())
                return 0;
        }

        /* Finally, try enabling the "fast A20 gate" */
        enable_a20_fast();
        if (a20_test())
            return 0;
    }

    return -1;
}
