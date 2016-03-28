/*
 * File name: kernel.c
 * Description: Main kernel source file. Contains the entry point.
 *
 * * * */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <boot/multiboot.h>
#include <integral/kernel.h>
#include <integral/tty.h>
#include <io/port_io.h>

#include <hal/cpu.h>

static void init_gdt(void);

void kernel_init(multiboot_info_t *multiboot_info, uint32_t bootloader_magic) {
    if(bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("Not loaded by a multiboot-compliant bootloader.");
    }
    init_terminal();

    printf("\\[2Integral OS\\X\n");
    printf("Preparing the operating system environment...\n\n");

    init_gdt();

    return;
}

void panic(const char *message) {
    int32_t eax;
    int32_t ebx;
    int32_t ecx;
    int32_t edx;

    asm volatile("movl %%eax, %0" : "=r"(eax));
    asm volatile("movl %%ebx, %0" : "=r"(ebx));
    asm volatile("movl %%ecx, %0" : "=r"(ecx));
    asm volatile("movl %%edx, %0" : "=r"(edx));


    init_terminal();

    write("[\\[4KERNEL PANIC\\X] The system has failed:\n");
    printf("%s\n", message);
    printf("EAX: 0x%08X ECX: 0x%08X\n", eax, ecx);
    printf("EBX: 0x%08X EDX: 0x%08X", ebx, edx);

    asm volatile("cli");
    asm volatile("hlt");
}

static void init_gdt(void) {
    printf("Setting up the Global Descriptor Table... ");
    gdt_descriptor_t descriptor = init_global_descriptor_table();
    printf("\\[2Loaded @\\X 0x%08X\n", descriptor.address);
}
