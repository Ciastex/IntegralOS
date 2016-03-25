/*
 * File name: vga.h
 * Description: X86 VGA interface.
 *
 * * * */

#ifndef VGA_H_
#define VGA_H_

#include <stdint.h>

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 80

typedef enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GRAY = 7,
    COLOR_DARK_GRAY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15
} vga_color_t;

typedef uint8_t vga_attrib_t;
typedef uint16_t vga_entry_t;
typedef uint32_t vga_coord_t;

void initialize_screen(void);
void clear_screen(void);
void draw_char_at(char, vga_coord_t x, vga_coord_t y);

#endif /* VGA_H_ */
