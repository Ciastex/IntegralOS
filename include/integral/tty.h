/*
 * File name: tty.h
 * Description: Provides the interface for TTY.
 *
 * * * */

#ifndef TTY_H_
#define TTY_H_

#include <stdbool.h>
#include <display/vga.h>

#define TTY_MAX_TERMINALS 8

#define TTY_SET_COLOR_BUFFER 0x01
#define TTY_SET_COLOR_STATUSBAR 0x02
#define TTY_SET_COLOR_FOREGROUND 0x04
#define TTY_SET_COLOR_BACKGROUND 0x08
#define TTY_UPDATE_ATTRIBUTES 0x10

typedef uint8_t tty_setcolor_flags_t;

typedef struct tty_cursor {
    vga_cursor_info_t info;
    bool enabled;
} tty_cursor_t;

typedef struct tty_colors {
    vga_color_info_t buffer;
    vga_color_info_t statusbar;
} tty_colors_t;

typedef struct tty_attributes {
    vga_attrib_t buffer;
    vga_attrib_t statusbar;
} tty_attributes_t;

typedef struct tty_terminal_info {
    uint8_t index;
    uint16_t* buffer;
    tty_cursor_t cursor;
    tty_colors_t colors;
    tty_attributes_t attributes;
} tty_terminal_info_t;

typedef void (tty_post_init_callback_t)(tty_terminal_info_t* terminal);
typedef void (tty_on_switch_callback_t)(tty_terminal_info_t* current, tty_terminal_info_t* previous);

void tty_init_terminals(tty_post_init_callback_t callback);
void tty_write_line(uint8_t terminal_index, const char* string);
void tty_write(uint8_t terminal_index, const char* string);
void tty_put_char(uint8_t terminal_index, char character);
void tty_set_colors(uint8_t terminal_index, tty_setcolor_flags_t flags, vga_color_t fg, vga_color_t bg);
void tty_reset_colors(uint8_t terminal_index, tty_setcolor_flags_t flags);
tty_colors_t tty_get_colors(uint8_t terminal_index);
void tty_set_statusbar_text(uint8_t terminal_index, const char* text);
void tty_set_terminal(uint8_t index);
void tty_update_hardware_cursor(tty_terminal_info_t* terminal, bool force);
void tty_read_line(char* buffer, size_t count);
tty_terminal_info_t* tty_get_terminal(uint8_t index);
tty_terminal_info_t* tty_get_current_terminal(void);
uint8_t tty_get_current_terminal_index(void);
void tty_set_on_switch_callback(tty_on_switch_callback_t* callback);

#endif /* TTY_H_ */
