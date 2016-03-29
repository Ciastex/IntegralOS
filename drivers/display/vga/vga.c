/*
 * File name: vga.c
 * Description: Provides VGA driver implementation.
 *
 * * * */
#include <stdlib.h>
#include <string.h>

#include <display/vga.h>
#include <io/port_io.h>

static vga_attrib_t make_vga_attribute(vga_color_t, vga_color_t);
static vga_entry_t make_vga_entry(char, vga_attrib_t);

static uint16_t *terminal_buffer = (uint16_t *)VGA_BUFFER;

static vga_color_t foreground_color;
static vga_color_t background_color;
static vga_attrib_t screen_attribute;

static vga_coord_t cursor_x = 0;
static vga_coord_t cursor_y = 0;

void initialize_screen_defaults(void) {
    set_cursor_position(0, 0);

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

void put_char_at_cursor(char character) {
    const vga_coord_t target_coord = cursor_y * VGA_WIDTH + cursor_x;
    terminal_buffer[target_coord] = make_vga_entry(character, screen_attribute);
}

void set_vga_colors(vga_color_t foreground, vga_color_t background) {
    foreground_color = foreground;
    background_color = background;

    screen_attribute = make_vga_attribute(foreground, background);
}

void set_vga_foreground(vga_color_t foreground) {
    foreground_color = foreground;
    set_vga_colors(foreground, background_color);
}

void set_vga_background(vga_color_t background) {
    background_color = background;
    set_vga_colors(foreground_color, background);
}

void reset_vga_colors(void) {
    set_vga_colors(COLOR_WHITE, COLOR_BLACK);
}

vga_color_info_t get_current_vga_colors(void) {
    vga_color_info_t color_info;
    memset(&color_info, 0, sizeof(vga_color_info_t));

    color_info.foreground = (screen_attribute >> 4) & 0x0F;
    color_info.background = screen_attribute & 0x0F;

    return color_info;
}

void set_cursor_position(vga_coord_t x, vga_coord_t y) {
    cursor_x = x;
    cursor_y = y;

    vga_coord_t target_coord = (y * VGA_WIDTH) + x;

    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_LOW);
    outb(VGA_DATA_PORT, (uint8_t)(target_coord & 0xFF));

    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_HIGH);
    outb(VGA_DATA_PORT, (uint8_t)((target_coord >> 8) & 0xFF));
}

vga_cursor_info_t get_cursor_position(void) {
    vga_cursor_info_t cursor_info;
    memset(&cursor_info, 0, sizeof(vga_cursor_info_t));

    cursor_info.x_pos = cursor_x;
    cursor_info.y_pos = cursor_y;

    return cursor_info;
}


void scroll(void)
{
   for (size_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++)
   {
       terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];
   }

   for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
   {
       terminal_buffer[i] = make_vga_entry(' ', screen_attribute);
   }
}

void disable_cursor(void) {
    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_START);
    uint8_t cursor_state = inb(VGA_DATA_PORT);
    SET_BIT(cursor_state, 5);
    outb(VGA_DATA_PORT, cursor_state);
}

void enable_cursor(void) {
    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_START);
    uint8_t cursor_state = inb(VGA_DATA_PORT);
    CLEAR_BIT(cursor_state, 5);
    outb(VGA_DATA_PORT, cursor_state);
}

static vga_attrib_t make_vga_attribute(vga_color_t foreground, vga_color_t background) {
    return (vga_attrib_t)(foreground | (background << 4));
}

static vga_entry_t make_vga_entry(char character, vga_attrib_t attribute) {
    uint16_t character16 = character;
    uint16_t attribute16 = attribute;

    return (vga_entry_t)(character16 | (attribute16 << 8));
}
