#include <stdarg.h>

#include <kernel/ioport.hpp>
#include <kernel/panic.hpp>
#include <kernel/console.hpp>

/**
 * Sick PC logo
 */
static const char *sickpc = " \
                               _______      \n\
                               |.-----.|    \n\
                               ||x . x||    \n\
                               ||_.-._||    \n\
                               `--)-(--`    \n\
                              __[=== o]___  \n\
                             |:::::::::::|\\ \n\
                             `-=========-`()\n\
                                M. O. S.\n\n";

/**
 * Disclaimer
 */
static const char *disclamer =
    "We apologize, MOS has encountered a problem and has been shut down\n\
to prevent damage to your computer. Any unsaved work might be lost.\n\
We are sorry for the inconvenience this might have caused.\n\n\
Please report the following information and restart your computer.\n\
The system has been halted.\n\n";

void kernel::panic(const char *__restrict fmt, ...)
{
    // Disable interrupts
    cli();

    // Clear system console screen
    console.set_bg_color(VGA_COLOR_BLUE);
    console.set_fg_color(VGA_COLOR_WHITE);
    console.clrscr();

    console.printf(sickpc);
    console.printf(disclamer);
    console.printf("*** STOP: ");

    va_list ap;

    va_start(ap, fmt);
    console.vprintf(fmt, ap);
    va_end(ap);

    // Hang CPU
    /** TODO: Hang all CPUs */
    hang();
}