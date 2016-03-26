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
static vga_color_t determine_output_color(char character);

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
        if(string[i] == '\\') {
            if(i + 1 >= length) {
                put_char(string[i]);
            } else {
                if(string[i + 1] == '\\') {
                    i++;
                    put_char(string[i]);
                    continue;
                }
                i++;
                switch(string[i]) {
                    case '[':
                        if(i + 1 >= length) {
                            put_char(string[i]);
                        } else {
                            i++;
                            set_vga_foreground(determine_output_color(string[i]));
                        }
                        break;
                    case ']':
                        if(i + 1 >= length) {
                            put_char(string[i]);
                        } else {
                            i++;
                            set_vga_background(determine_output_color(string[i]));
                        }
                        break;
                    case 'X':
                        reset_vga_colors();
                        break;
                    default:
                        put_char(string[i]);
                        break;
                }
            }
        } else {
            put_char(string[i]);
        }
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

static vga_color_t determine_output_color(char character) {
    switch(character) {
        case '0':
            return COLOR_BLACK;
        case '1':
            return COLOR_BLUE;
        case '2':
            return COLOR_GREEN;
        case '3':
            return COLOR_CYAN;
        case '4':
            return COLOR_RED;
        case '5':
            return COLOR_MAGENTA;
        case '6':
            return COLOR_BROWN;
        case '7':
            return COLOR_LIGHT_GRAY;
        case '8':
            return COLOR_DARK_GRAY;
        case '9':
            return COLOR_LIGHT_BLUE;
        case 'A':
            return COLOR_LIGHT_GREEN;
        case 'B':
            return COLOR_LIGHT_CYAN;
        case 'C':
            return COLOR_LIGHT_RED;
        case 'D':
            return COLOR_LIGHT_MAGENTA;
        case 'E':
            return COLOR_LIGHT_BROWN;
        case 'F':
            return COLOR_WHITE;
        default:
            return COLOR_BLACK;
    }
}

