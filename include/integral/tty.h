/*
 * File name: tty.h
 * Description: Provides the interface for TTY.
 *
 * * * */

#ifndef TTY_H_
#define TTY_H_

#include <display/vga.h>

void init_terminal(void);
void write_line(const char *string);
void write(const char *string);
void put_char(char character);
void set_statusbar_text(const char *text);

#endif /* TTY_H_ */
