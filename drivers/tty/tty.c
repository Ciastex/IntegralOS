/*
 * File name: tty.c
 * Description: Provides a TTY implementation.
 *
 * * * */
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <integral/tty.h>
#include <display/vga.h>
#include <io/keyboard/keyboard.h>

static bool _reading_line = false;
static char* _readline_buffer;
static size_t _readline_count = 0;
static size_t _readline_max = 0;

static void _tty_put_newline(vga_cursor_info_t *cursor_info);
static void _tty_keypress_handler(key_info_t key_info);

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

    kbd_set_pressed_callback(_tty_keypress_handler);
}

void tty_write_line(const char *string) {
    tty_write(string);
    tty_put_char('\n');
}

void tty_read_line(char* buffer, size_t count) {
    memset(buffer, 0, count);
    _readline_buffer = buffer;
    _readline_max = count;
    _readline_count = 0;
    _reading_line = true;

    while(_reading_line);
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
    } else if(character == '\b') {
        if(cursor_info.x_pos - 1 < 0) {
            if(cursor_info.y_pos > 0) {
                cursor_info.x_pos = VGA_WIDTH - 1;
                cursor_info.y_pos -= 1;
            }
            else { return; }
        }
        else {
            cursor_info.x_pos -= 1;
        }
        vga_set_cursor_position(cursor_info.x_pos, cursor_info.y_pos);
        vga_put_char_at_cursor(' ');
        return;
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

static void _tty_keypress_handler(key_info_t key_info) {
    if(_reading_line) {
        if(key_info.key_code == VK_RETURN) {
            _readline_buffer[_readline_count] = 0;
            _reading_line = false;
        }
        else if(key_info.key_code == VK_BACKSPACE && _readline_count > 0)
        {
            _readline_buffer[_readline_count--] = 0;
            tty_put_char('\b');
        }
        else if(key_info.character >= 0x20 && key_info.character <= 0x7E) {
            if(_readline_count >= _readline_max - 2) return;

            _readline_buffer[_readline_count++] = key_info.character;
            tty_put_char(key_info.character);
        }
    }
}

