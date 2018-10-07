/*
 * File name: vga.c
 * Description: Provides VGA driver implementation.
 *
 * * * */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <display/vga.h>
#include <io/port_io.h>

static vga_entry_t _vga_make_entry(char character, vga_attrib_t attribute);

void vga_clear_screen(uint16_t* buffer, vga_attrib_t attribute) {
    uint16_t* target_buffer = (uint16_t*)(VGA_BASE_BUFFER + (uint32_t)buffer);

    for(vga_coord_t y = 0; y < VGA_HEIGHT; y++) {
        for(vga_coord_t x = 0; x < VGA_WIDTH; x++) {
            const vga_coord_t target_coord = y * VGA_WIDTH + x;
            target_buffer[target_coord] = _vga_make_entry(' ', attribute);
        }
    }
}

void vga_put_char_at(uint16_t* buffer, vga_coord_t x, vga_coord_t y, vga_attrib_t attribute, char character) {
    uint16_t* target_buffer = (uint16_t*)(VGA_BASE_BUFFER + (uint32_t)buffer);

    const vga_coord_t target_coord = y * VGA_WIDTH + x;
    target_buffer[target_coord] = _vga_make_entry(character, attribute);
}

void vga_put_char_at_cursor(uint16_t* buffer, vga_cursor_info_t cursor, vga_attrib_t attribute, char character) {
    uint16_t* target_buffer = (uint16_t*)(VGA_BASE_BUFFER + (uint32_t)buffer);

    const vga_coord_t target_coord = cursor.y * VGA_WIDTH + cursor.x;
    target_buffer[target_coord] = _vga_make_entry(character, attribute);
}

void vga_set_cursor_position(uint16_t* buffer, vga_coord_t x, vga_coord_t y) {
    vga_coord_t target_coord = ((uint32_t)buffer / 2) + (y * VGA_WIDTH) + x;

    // apparently vga is retarded here too, so we have to provide
    // a base address and divide it by 2, since addressing is 32-bit as well
    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_LOW);
    outb(VGA_DATA_PORT, (uint8_t)(target_coord & 0xFF));

    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_HIGH);
    outb(VGA_DATA_PORT, (uint8_t)((target_coord >> 8) & 0xFF));
}

void vga_set_cursor(uint16_t* buffer, vga_cursor_info_t* cursor) {
    vga_set_cursor_position(buffer, cursor->x, cursor->y);
}

void vga_scroll(uint16_t* buffer, size_t window_height, vga_attrib_t attribute) {
    uint16_t* target_buffer = (uint16_t*)(VGA_BASE_BUFFER + (uint32_t)buffer);

   for (size_t i = 0; i < window_height * VGA_WIDTH; i++) {
       target_buffer[i] = target_buffer[i + VGA_WIDTH];
   }

   for (size_t i = window_height * VGA_WIDTH; i < (window_height + 1) * VGA_WIDTH; i++) {
       target_buffer[i] = _vga_make_entry(' ', attribute);
   }
}

void vga_disable_cursor(void) {
    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_START);
    uint8_t cursor_state = inb(VGA_DATA_PORT);
    SET_BIT(cursor_state, 5);
    outb(VGA_DATA_PORT, cursor_state);
}

void vga_enable_cursor(void) {
    outb(VGA_BASE_PORT, VGA_CMD_CURSOR_START);
    uint8_t cursor_state = inb(VGA_DATA_PORT);
    CLEAR_BIT(cursor_state, 5);
    outb(VGA_DATA_PORT, cursor_state);
}

void vga_set_address_space(uint16_t* address) {
    // need to divide address by 2 because vga
    // addresses display things using 32 bits instead of 16
    uint8_t base_low = (uint8_t)((uint32_t)address / 2 & 0xFF);
    uint8_t base_high = (uint8_t)(((uint32_t)address / 2 >> 8) & 0xFF);

    outb(VGA_BASE_PORT, VGA_CMD_MEMORY_LOW);
    outb(VGA_DATA_PORT, base_low);

    outb(VGA_BASE_PORT, VGA_CMD_MEMORY_HIGH);
    outb(VGA_DATA_PORT, base_high);
}

vga_attrib_t vga_make_attribute(vga_color_t foreground, vga_color_t background) {
    return (vga_attrib_t)(foreground | (background << 4));
}

static vga_entry_t _vga_make_entry(char character, vga_attrib_t attribute) {
    uint16_t character16 = character;
    uint16_t attribute16 = attribute;

    return (vga_entry_t)(character16 | (attribute16 << 8));
}
