/*
 * File name: vga.c
 * Description: x86 VGA implementation.
 *
 * * * */

#include "../include/vga.h"

static vga_attrib_t make_vga_attrib(vga_color_t, vga_color_t);
static vga_entry_t make_vga_entry(char, vga_attrib_t);

static int32_t was_initialized = 0;
static uint16_t *terminal_buffer = (uint16_t *)VGA_BUFFER;
static vga_attrib_t screen_attribute;

void initialize_screen(void) {
    if(!was_initialized) {
        screen_attribute = make_vga_attrib(COLOR_WHITE, COLOR_BLACK);
        was_initialized = 1;
    }
}

void clear_screen(void) {
    for(vga_coord_t y = 0; y < VGA_HEIGHT; y++) {
        for(vga_coord_t x = 0; x < VGA_WIDTH; x++) {
            const vga_coord_t target_coord = y * VGA_WIDTH + x;
            terminal_buffer[target_coord] = make_vga_entry(' ', screen_attribute);
        }
    }
}

void draw_char_at(char character, vga_coord_t x, vga_coord_t y) {
    const vga_coord_t target_coord = y * VGA_WIDTH + x;
    terminal_buffer[target_coord] = make_vga_entry(character, screen_attribute);
}

static vga_attrib_t make_vga_attrib(vga_color_t foreground, vga_color_t background) {
    return (vga_attrib_t)(foreground | (background << 4));
}

static vga_entry_t make_vga_entry(char character, vga_attrib_t attribute) {
    uint16_t character16 = character;
    uint16_t attribute16 = attribute;

    return (vga_entry_t)(character16 | (attribute16 << 8));
}
