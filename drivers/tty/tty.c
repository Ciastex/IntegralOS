/*
 * File name: tty.c
 * Description: Provides a TTY implementation.
 *
 * * * */

#include <stddef.h>
#include <string.h>

#include <integral/tty.h>
#include <display/vga.h>

static void put_char(char character);
static void put_newline(vga_cursor_info_t *cursor_info);

void init_terminal(void) {
    initialize_screen_defaults();
}

void write_line(const char *string) {
    write(string);
    put_char('\n');
}

void write(const char *string) {
    size_t length = strlen(string);

    for(size_t i = 0; i < length; i++) {
        put_char(string[i]);
    }
}

static void put_char(char character) {
    vga_cursor_info_t cursor_info = get_cursor_position();

    if(character == '\n') {
        put_newline(&cursor_info);
    } else if(character == '\r') {
        cursor_info.x_pos = 0;
    } else {
        put_char_at_cursor(character);

        if(cursor_info.x_pos < VGA_WIDTH) {
            cursor_info.x_pos += 1;
        } else {
            put_newline(&cursor_info);
        }
    }
    set_cursor_position(cursor_info.x_pos, cursor_info.y_pos);
}

static void put_newline(vga_cursor_info_t *cursor_info) {
    cursor_info->x_pos = 0;

    if(cursor_info->y_pos < VGA_HEIGHT) {
        cursor_info->y_pos += 1;
    } else {
        cursor_info->y_pos = VGA_HEIGHT - 1;
    }
}

