/*
 * File name: kernel.c
 * Description: Main kernel source file. Contains the entry point.
 *
 * * * */

#include <stdint.h>
#include <string.h>

#include <boot/multiboot.h>
#include <integral/kernel.h>
#include <integral/tty.h>

void kernel_init(multiboot_info_t *multiboot_info, uint32_t bootloader_magic) {
    init_terminal();

    if(bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        write("[\\[4BOOT FAILURE\\X] Not loaded by a multiboot-compliant bootloader.");
        return;
    }

    write("\\]E\\[1R\\]D\\[2A\\]C\\]B\\]A\\[3I\\]B\\[4N\\]9\\X\\[5B\\[6O\\[7W\\[8S \\[9A\\[AR\\[BE \\[CF\\[DU\\[EN");

    return;
}
