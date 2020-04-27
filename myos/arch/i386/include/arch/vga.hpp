#ifndef ARCH_I386_VGA_HPP
#define ARCH_I386_VGA_HPP

/**
 * VGA real mode memory location and attributes. 
 * Remember: We are in Mode 7, i.e. 80 columns by 
 * 25 lines of high resolution text characters.
 */

/** EGA mode memory map start address */
#define VGA_EGA_MEMORY 0xA0000

/** MDA mode memory map start address */
#define VGA_MDA_MEMORY 0xB0000

/** CGA mode memory map start address */
#define VGA_CGA_MEMORY 0xB8000

/** VGA text mode width and height **/
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/** VGA 16 bit color code */
enum VGAColor
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
};

#endif /* ARCH_I386_VGA_HPP */
