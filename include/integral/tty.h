/*
 * File name: tty.h
 * Description: Provides the interface for TTY.
 *
 * * * */

#ifndef TTY_H_
#define TTY_H_

#include <display/vga.h>

void tty_init_terminal(void);
void tty_write_line(const char *string);
void tty_write(const char *string);
void tty_put_char(char character);
void tty_set_statusbar_text(const char *text);
void tty_read_line(char* buffer, size_t count);

#endif /* TTY_H_ */
