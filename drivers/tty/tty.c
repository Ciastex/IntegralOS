/*
 * File name: tty.c
 * Description: Provides a TTY implementation.
 *
 * * * */
#include <stddef.h>
#include <string.h>

#include <integral/tty.h>
#include <display/vga.h>

static void _tty_put_newline(vga_cursor_info_t *cursor_info);
static vga_color_t _tty_determine_output_color(char character);

static vga_color_t statusbar_background = COLOR_GREEN;
static vga_color_t statusbar_foreground = COLOR_LIGHT_BROWN;

void tty_init_terminal(void) {
    vga_initialize_screen_defaults();

    vga_color_info_t original_colors = vga_get_current_colors();
    vga_set_foreground(statusbar_foreground);
    vga_set_background(statusbar_background);

    for(vga_coord_t x = 0; x <= VGA_WIDTH; x++) {
        vga_put_char_at_cursor(' ');
        vga_set_cursor_position(x, VGA_HEIGHT - 1);
    }
    vga_set_foreground(original_colors.foreground);
    vga_set_background(original_colors.background);

    vga_set_cursor_position(0, 0);
}

void tty_write_line(const char *string) {
    tty_write(string);
    tty_put_char('\n');
}

void tty_write(const char *string) {
    size_t length = strlen(string);

    for(size_t i = 0; i < length; i++) {
        if(string[i] == '\\') {
            if(i + 1 >= length) {
                tty_put_char(string[i]);
            } else {
                if(string[i + 1] == '\\') {
                    i++;
                    tty_put_char(string[i]);
                    continue;
                }
                i++;
                switch(string[i]) {
                    case '[':
                        if(i + 1 >= length) {
                            tty_put_char(string[i]);
                        } else {
                            i++;
                            vga_set_foreground(_tty_determine_output_color(string[i]));
                        }
                        break;
                    case ']':
                        if(i + 1 >= length) {
                            tty_put_char(string[i]);
                        } else {
                            i++;
                            vga_set_background(_tty_determine_output_color(string[i]));
                        }
                        break;
                    case 'X':
                        vga_reset_colors();
                        break;
                    default:
                        tty_put_char(string[i]);
                        break;
                }
            }
        } else {
            tty_put_char(string[i]);
        }
    }
}

void tty_put_char(char character) {
    vga_cursor_info_t cursor_info = vga_get_cursor_position();

    if(character == '\n') {
        _tty_put_newline(&cursor_info);
    } else if(character == '\r') {
        cursor_info.x_pos = 0;
    } else {
        vga_put_char_at_cursor(character);

        if(cursor_info.x_pos < VGA_WIDTH - 1) {
            cursor_info.x_pos += 1;
        } else {
            _tty_put_newline(&cursor_info);
        }
    }
    vga_set_cursor_position(cursor_info.x_pos, cursor_info.y_pos);
}

void tty_set_statusbar_text(const char *text) {
    vga_color_info_t original_colors = vga_get_current_colors();
    vga_set_foreground(statusbar_foreground);
    vga_set_background(statusbar_background);

    size_t length = strlen(text);
    for(size_t x = 0; x < length; x++) {
        vga_put_char_at(text[x], x, VGA_HEIGHT - 1);

        if(x > VGA_WIDTH) {
            break;
        }
    }
    vga_set_foreground(original_colors.foreground);
    vga_set_background(original_colors.background);
}

static void _tty_put_newline(vga_cursor_info_t *cursor_info) {
    cursor_info->x_pos = 0;

    if(cursor_info->y_pos < VGA_HEIGHT - 2) {
        cursor_info->y_pos += 1;
    } else {
        cursor_info->y_pos = VGA_HEIGHT - 2;
        vga_scroll(VGA_HEIGHT - 2);
    }
}

static vga_color_t _tty_determine_output_color(char character) {
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

