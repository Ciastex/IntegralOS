/*
 * File name: kernel.c
 * Description: Main kernel source file. Contains the entry point.
 *
 * * * */

#include <stdint.h>
#include <string.h>

#include <integral/kernel.h>
#include <integral/tty.h>

void kernel_init(void) {
    init_terminal();
    write_line("This is a terminal test.\n\n\nMeow.");

    return;
}
