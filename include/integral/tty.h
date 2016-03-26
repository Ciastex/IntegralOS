/*
 * File name: tty.h
 * Description: Provides the interface for TTY.
 *
 * * * */

#ifndef TTY_H_
#define TTY_H_

void init_terminal(void);
void write_line(const char *string);
void write(const char *string);

#endif /* TTY_H_ */
