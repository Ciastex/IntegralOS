/*
 * File name: vga.c
 * Description: Provides VGA driver implementation.
 *
 * * * */
#include <stddef.h>
#include <string.h>

#include <display/vga.h>

static vga_attrib_t make_vga_attribute(vga_color_t, vga_color_t);
static vga_entry_t make_vga_entry(char, vga_attrib_t);

static uint16_t *terminal_buffer = (uint16_t *)VGA_BUFFER;
static vga_attrib_t screen_attribute = NULL;

void initialize_screen_defaults(void) {
    screen_attribute = make_vga_attribute(COLOR_WHITE, COLOR_BLACK);
    clear_screen();
}

void clear_screen(void) {
    for(vga_coord_t y = 0; y < VGA_HEIGHT; y++) {
        for(vga_coord_t x = 0; x < VGA_WIDTH; x++) {
            const vga_coord_t target_coord = y * VGA_WIDTH + x;
            terminal_buffer[target_coord] = make_vga_entry(' ', screen_attribute);
        }
    }
}

void put_char_at(char character, vga_coord_t x, vga_coord_t y) {
    const vga_coord_t target_coord = y * VGA_WIDTH + x;
    terminal_buffer[target_coord] = make_vga_entry(character, screen_attribute);
}

void set_vga_colors(vga_color_t foreground, vga_color_t background) {
    screen_attribute = make_vga_attribute(foreground, background);
}

vga_color_info_t *get_current_vga_colors(void) {
    vga_color_info_t color_info;
    memset(&color_info, 0, sizeof(vga_color_info_t));

    color_info.foreground = screen_attribute >> 4;
    color_info.background = screen_attribute & 0x0F;

    return &color_info;
}

static vga_attrib_t make_vga_attribute(vga_color_t foreground, vga_color_t background) {
    return (vga_attrib_t)(foreground | (background << 4));
}

static vga_entry_t make_vga_entry(char character, vga_attrib_t attribute) {
    uint16_t character16 = character;
    uint16_t attribute16 = attribute;

    return (vga_entry_t)(character16 | (attribute16 << 8));
}
