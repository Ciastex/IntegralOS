/*
 * File name: vga.h
 * Description: X86 VGA driver interface.
 *
 * * * */

#ifndef VGA_H_
#define VGA_H_

#include <stdint.h>

#define VGA_BASE_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_CMD_CURSOR_START 0x0A
#define VGA_CMD_CURSOR_END 0x0B
#define VGA_CMD_CURSOR_HIGH 0x0E
#define VGA_CMD_CURSOR_LOW 0x0F
#define VGA_CMD_MEMORY_LOW 0x0D
#define VGA_CMD_MEMORY_HIGH 0x0C

#define VGA_BASE_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

typedef uint8_t vga_attrib_t;
typedef uint16_t vga_entry_t;
typedef uint16_t vga_coord_t;

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

typedef struct vga_color_info {
    vga_color_t foreground;
    vga_color_t background;
} vga_color_info_t;

typedef struct vga_cursor_info {
    vga_coord_t x;
    vga_coord_t y;
} vga_cursor_info_t;

void vga_clear_screen(uint16_t* buffer, vga_attrib_t attribute);
void vga_put_char_at(uint16_t* buffer, vga_coord_t x, vga_coord_t y, vga_attrib_t attribute, char character);
void vga_put_char_at_cursor(uint16_t* buffer, vga_cursor_info_t cursor, vga_attrib_t attribute, char character);
void vga_scroll(uint16_t* buffer, size_t window_height, vga_attrib_t attribute);
void vga_disable_cursor(void);
void vga_enable_cursor(void);
void vga_set_cursor_position(uint16_t* buffer, vga_coord_t x, vga_coord_t y);
void vga_set_cursor(uint16_t* buffer, vga_cursor_info_t* cursor);
void vga_set_address_space(uint16_t* address);
vga_attrib_t vga_make_attribute(vga_color_t foreground, vga_color_t background);


#endif /* VGA_H_ */
