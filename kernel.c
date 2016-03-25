/*
 * File name: kernel.c
 * Description: Main kernel source file. Contains the entry point.
 *
 * * * */

#include <stdint.h>
#include <integral/kernel.h>

#include "arch/x86/include/vga.h"

void kernel_init(void) {
    initialize_screen();
    clear_screen();

    draw_char_at('X', 40, 12);

    return;
}
