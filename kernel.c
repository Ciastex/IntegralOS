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
#include <integral/panic.h>
#include <integral/tty.h>
#include <display/vga.h>
#include <io/port_io.h>
#include <io/8259a/pic.h>

#include <hal/cpu.h>
#include <hal/irq.h>

static void print_welcome_screen(void);
static void init_gdt(void);
static void init_idt(void);
static void init_pic(void);
static void pit_callback(irq_info_t *irq_info);

void kernel_init(multiboot_info_t *multiboot_info, uint32_t bootloader_magic) {
    init_terminal();

    if(bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kpanic("Not loaded by a multiboot-compliant bootloader.");
    } else {
        print_welcome_screen();

        init_gdt();
        init_idt();

        set_irq_handler(0, (uint32_t)&pit_callback);

        init_pic();

        for(;;);
    }
}

static void pit_callback(irq_info_t *irq_info) {
    printf("kurwa chuj ");
}

static void print_welcome_screen(void) {
    printf("\\[AIntegral OS kernel\\X v%s\n%s\n", INTEGRAL_VERSION, INTEGRAL_COPYRIGHT);
    for(size_t i = 0; i < 40; i++) {
        printf("-");
    }
    printf("\n");
    printf("Preparing the operating system environment...\n\n");
}

static void init_gdt(void) {
    printf("Setting up the Global Descriptor Table... ");
    gdt_descriptor_t descriptor = init_global_descriptor_table();
    printf("\\[2Loaded @\\X 0x%08X\n", descriptor.address);
}

static void init_idt(void) {
    printf("Setting up the Interrupt Descriptor Table... ");
    idt_descriptor_t descriptor = init_interrupt_descriptor_table();
    printf("\\[2Loaded @\\X 0x%08X\n", descriptor.address);
}

static void init_pic(void) {
    printf("Initializing 8259A PIC... ");
    pic_remap(32, 40);
    printf("\\[2OK\\X\nIRQ_VEC_START_\\[AMASTER\\X = 32\nIRQ_VEC_START_\\[ASLAVE\\X = 40\n");
}
