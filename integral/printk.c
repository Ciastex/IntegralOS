#include <stdio.h>
#include <stdarg.h>

#include <integral/tty.h>

uint32_t printk(uint8_t terminal_index, char* str, ...) {
    char *output = { 0 };
    uint32_t length = 0;

    va_list args;
    va_start(args, str);

    length = vsprintf(output, str, args);

    va_end(args);
    tty_write(terminal_index, (const char *)output);

    return length;
}
