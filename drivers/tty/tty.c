/*
 * File name: tty.c
 * Description: Provides a TTY implementation.
 *
 * * * */
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <integral/tty.h>
#include <display/vga.h>
#include <io/keyboard/keyboard.h>

static const vga_color_t TTY_DEFAULT_BUFFER_BG = COLOR_BLACK;
static const vga_color_t TTY_DEFAULT_BUFFER_FG = COLOR_WHITE;
static const vga_color_t TTY_DEFAULT_STATUSBAR_BG = COLOR_GREEN;
static const vga_color_t TTY_DEFAULT_STATUSBAR_FG = COLOR_LIGHT_BROWN;

static bool _reading_line = false;
static char* _readline_buffer;
static size_t _readline_count = 0;
static size_t _readline_max = 0;
static uint8_t _readline_framebuffer_index = 0;

static void _tty_put_newline(tty_terminal_info_t* terminal);
static void _tty_keypress_handler(key_info_t key_info);
static vga_color_t _tty_determine_output_color(char character);

static uint8_t current_terminal_index = 0;
static tty_terminal_info_t terminals[TTY_MAX_TERMINALS] = { 0 };

void tty_init_terminals(void) {
    for(uint8_t i = 0; i < TTY_MAX_TERMINALS; i++) {
        vga_cursor_info_t cursor_info;
        tty_cursor_t cursor;
        vga_color_info_t buffer_colors;
        vga_color_info_t statusbar_colors;
        tty_colors_t colors;
        tty_attributes_t attributes;
        tty_terminal_info_t terminal;

        cursor_info.x = 0;
        cursor_info.y = 0;

        cursor.info = cursor_info;
        cursor.enabled = true;

        buffer_colors.foreground = TTY_DEFAULT_BUFFER_FG;
        buffer_colors.background = TTY_DEFAULT_BUFFER_BG;

        statusbar_colors.foreground = TTY_DEFAULT_STATUSBAR_FG;
        statusbar_colors.background = TTY_DEFAULT_STATUSBAR_BG;

        colors.buffer = buffer_colors;
        colors.statusbar = statusbar_colors;

        attributes.buffer = vga_make_attribute(TTY_DEFAULT_BUFFER_FG, TTY_DEFAULT_BUFFER_BG);
        attributes.statusbar = vga_make_attribute(TTY_DEFAULT_STATUSBAR_FG, TTY_DEFAULT_STATUSBAR_BG);

        terminal.index = i;
        terminal.buffer = (uint16_t*)((VGA_WIDTH*VGA_HEIGHT*2)*i);
        terminal.cursor = cursor;
        terminal.colors = colors;
        terminal.attributes = attributes;

        tty_update_hardware_cursor(&terminal);
        terminals[terminal.index] = terminal;

        vga_clear_screen(terminal.buffer, terminal.attributes.buffer);
    }
    kbd_set_pressed_callback(_tty_keypress_handler);
}

void tty_write_line(uint8_t terminal_index, const char *string) {
    tty_write(terminal_index, string);
    tty_put_char(terminal_index, '\n');
}

void tty_read_line(char* buffer, size_t count) {
    memset(buffer, 0, count);
    _readline_buffer = buffer;
    _readline_max = count;
    _readline_count = 0;
    _reading_line = true;
    _readline_framebuffer_index = current_terminal_index;

    while(_reading_line);
}

void tty_write(uint8_t terminal_index, const char *string) {
    if(terminal_index == TTY_CURRENT_TERMINAL)
        terminal_index = current_terminal_index;

    size_t length = strlen(string);

    for(size_t i = 0; i < length; i++) {
        if(string[i] == '\\') {
            if(i + 1 >= length) {
                tty_put_char(terminal_index, string[i]);
            } else {
                if(string[i + 1] == '\\') {
                    i++;
                    tty_put_char(terminal_index, string[i]);
                    continue;
                }
                i++;
                switch(string[i]) {
                    case '[':
                        if(i + 1 >= length) {
                            tty_put_char(terminal_index, string[i]);
                        } else {
                            i++;

                            vga_color_t fg_color = _tty_determine_output_color(string[i]);
                            tty_set_colors(
                                terminal_index,
                                TTY_SET_COLOR_BUFFER | TTY_SET_COLOR_FOREGROUND | TTY_UPDATE_ATTRIBUTES,
                                fg_color, (vga_color_t)NULL
                            );
                        }
                        break;
                    case ']':
                        if(i + 1 >= length) {
                            tty_put_char(terminal_index, string[i]);
                        } else {
                            i++;

                            vga_color_t bg_color = _tty_determine_output_color(string[i]);
                            tty_set_colors(
                                terminal_index,
                                TTY_SET_COLOR_BUFFER | TTY_SET_COLOR_FOREGROUND | TTY_UPDATE_ATTRIBUTES,
                                (vga_color_t)NULL, bg_color
                            );
                        }
                        break;
                    case 'X':
                        tty_reset_colors(
                            terminal_index,
                            TTY_SET_COLOR_BUFFER | TTY_SET_COLOR_FOREGROUND | TTY_SET_COLOR_BACKGROUND
                        );
                        break;
                    default:
                        tty_put_char(terminal_index, string[i]);
                        break;
                }
            }
        } else {
            tty_put_char(terminal_index, string[i]);
        }
    }
}

void tty_put_char(uint8_t terminal_index, char character) {
    tty_terminal_info_t* terminal = tty_get_terminal(terminal_index);

    if(character == '\n') {
        _tty_put_newline(terminal);
    } else if(character == '\r') {
        terminal->cursor.info.x = 0;
    } else if(character == '\b') {
        if(terminal->cursor.info.x - 1 < 0) {
            if(terminal->cursor.info.y > 0) {
                terminal->cursor.info.x = VGA_WIDTH - 1;
                terminal->cursor.info.y -= 1;
            }
            else { return; }
        }
        else {
            terminal->cursor.info.x -= 1;
        }
        vga_put_char_at(terminal->buffer, terminal->cursor.info.x, terminal->cursor.info.y, terminal->attributes.buffer, ' ');
    } else {
        vga_put_char_at(terminal->buffer, terminal->cursor.info.x, terminal->cursor.info.y, terminal->attributes.buffer, character);

        if(terminal->cursor.info.x < VGA_WIDTH - 1) {
            terminal->cursor.info.x += 1;
        } else {
            _tty_put_newline(terminal);
        }
    }

    tty_update_hardware_cursor(terminal);
}

void tty_set_statusbar_text(uint8_t terminal_index, const char *text) {
    tty_terminal_info_t* terminal = tty_get_terminal(terminal_index);

    size_t length = strlen(text);
    for(size_t x = 0; x < length; x++) {
        vga_put_char_at(terminal->buffer, x, VGA_HEIGHT - 1, terminal->attributes.statusbar, text[x]);

        if(x > VGA_WIDTH) {
            break;
        }
    }

    for(size_t x = length; x < VGA_WIDTH; x++) {
        vga_put_char_at(terminal->buffer, x, VGA_HEIGHT - 1, terminal->attributes.statusbar, ' ');
    }
}

void tty_update_hardware_cursor(tty_terminal_info_t* terminal) {
    if(terminal->cursor.enabled)
        vga_enable_cursor();
    else vga_disable_cursor();

    if(terminal->index == current_terminal_index) {
        vga_set_cursor_position(terminal->buffer, terminal->cursor.info.x, terminal->cursor.info.y);
    }
}

void tty_set_colors(uint8_t terminal_index, tty_setcolor_flags_t flags, vga_color_t fg, vga_color_t bg) {
    if(!flags) return;

    tty_terminal_info_t* terminal = tty_get_terminal(terminal_index);
    vga_color_info_t* colors;

    if(flags & TTY_SET_COLOR_BUFFER) {
        colors = &terminal->colors.buffer;
    } else if(flags & TTY_SET_COLOR_STATUSBAR) {
        colors = &terminal->colors.statusbar;
    }

    if(flags & TTY_SET_COLOR_FOREGROUND) {
        colors->foreground = fg;
    }

    if(flags & TTY_SET_COLOR_BACKGROUND) {
        colors->background = bg;
    }

    if(flags & TTY_UPDATE_ATTRIBUTES) {
        tty_attributes_t attributes;

        attributes.buffer = vga_make_attribute(
            terminal->colors.buffer.foreground,
            terminal->colors.buffer.background
        );

        attributes.statusbar = vga_make_attribute(
            terminal->colors.statusbar.foreground,
            terminal->colors.statusbar.background
        );

        terminal->attributes = attributes;
    }
}

void tty_reset_colors(uint8_t terminal_index, tty_setcolor_flags_t flags) {
    vga_color_t fg, bg;

    if(flags & TTY_SET_COLOR_BUFFER) {
        fg = TTY_DEFAULT_BUFFER_FG;
        bg = TTY_DEFAULT_BUFFER_BG;
    }

    if(flags & TTY_SET_COLOR_STATUSBAR) {
        fg = TTY_DEFAULT_STATUSBAR_FG;
        bg = TTY_DEFAULT_STATUSBAR_BG;
    }

    tty_set_colors(terminal_index, flags | TTY_UPDATE_ATTRIBUTES, fg, bg);
}

void tty_set_terminal(uint8_t index) {
    if(current_terminal_index == index || index >= TTY_MAX_TERMINALS) return;
    current_terminal_index = index;

    tty_terminal_info_t* terminal = tty_get_terminal(index);
    vga_set_address_space(terminal->buffer);

    tty_update_hardware_cursor(terminal);
}

tty_terminal_info_t* tty_get_terminal(uint8_t index) {
    return &terminals[index];
}

static void _tty_put_newline(tty_terminal_info_t *terminal) {
    terminal->cursor.info.x = 0;

    if(terminal->cursor.info.y < VGA_HEIGHT - 2) {
        terminal->cursor.info.y += 1;
    } else {
        terminal->cursor.info.y = VGA_HEIGHT - 2;
        vga_scroll(terminal->buffer, VGA_HEIGHT - 2, terminal->attributes.buffer);
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
    if(_reading_line && _readline_framebuffer_index == current_terminal_index) {
        if(key_info.key_code == VK_RETURN) {
            _readline_buffer[_readline_count] = 0;
            _reading_line = false;
        }
        else if(key_info.key_code == VK_BACKSPACE && _readline_count > 0)
        {
            _readline_buffer[_readline_count--] = 0;
            tty_put_char(_readline_framebuffer_index, '\b');
        }
        else if(key_info.character >= 0x20 && key_info.character <= 0x7E) {
            if(_readline_count >= _readline_max - 2) return;

            _readline_buffer[_readline_count++] = key_info.character;
            tty_put_char(_readline_framebuffer_index, key_info.character);
        }
    }

    switch(key_info.key_code)
    {
        case VK_F1: tty_set_terminal(0); break;
        case VK_F2: tty_set_terminal(1); break;
        case VK_F3: tty_set_terminal(2); break;
        case VK_F4: tty_set_terminal(3); break;
        case VK_F5: tty_set_terminal(4); break;
        case VK_F6: tty_set_terminal(5); break;
        case VK_F7: tty_set_terminal(6); break;
        case VK_F8: tty_set_terminal(7); break;
        default: break;
    }
}
