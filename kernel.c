/*
 * File name: kernel.c
 * Description: Main kernel source file. Contains the entry point.
 *
 * * * */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <boot/multiboot.h>

#include <integral/exception.h>
#include <integral/trap.h>
#include <integral/kernel.h>
#include <integral/panic.h>
#include <integral/tty.h>
#include <integral/timer.h>

#include <hal/idt.h>
#include <hal/gdt.h>
#include <hal/irq.h>
#include <hal/bda.h>

#include <display/vga.h>

#include <io/port_io.h>
#include <io/8259a/pic.h>
#include <io/8259a/pit.h>
#include <io/keyboard/keyboard.h>

static void _ke_print_welcome_screen(void);
static void _ke_init_gdt(void);
static void _ke_init_idt(void);
static void _ke_init_pic(void);
static void _ke_init_pit(void);
static void _ke_init_kbd(void);
static void _ke_pit_callback(void);
static void _ke_tty_callback(tty_terminal_info_t* terminal);

void kernel_init(multiboot_info_t *multiboot_info, uint32_t bootloader_magic) {
    tty_init_terminals(_ke_tty_callback);
    tty_set_statusbar_text(0, "Hello, world.");

    if(bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kpanic("Not loaded by a multiboot-compliant bootloader.", bootloader_magic, 0, 0);
    } else {
        _ke_print_welcome_screen();

        _ke_init_gdt();
        _ke_init_idt();
        _ke_init_pic();
        _ke_init_pit();
        _ke_init_kbd();

        pit_set_callback((uintptr_t)&_ke_pit_callback);
        for(;;);
    }
}

static void _ke_print_welcome_screen(void) {
    printf("\\[AIntegral OS kernel\\X v%s\n%s\n", INTEGRAL_VERSION, INTEGRAL_COPYRIGHT);
    for(size_t i = 0; i < 40; i++) {
        printf("-");
    }
    printf("\n");
    printf("Preparing the operating system environment...\n");
    printf("Use keys F1-F8 to switch between virtual terminals.\n\n");
}

static void _ke_init_gdt(void) {
    gdt_descriptor_t descriptor = gdt_init_global_descriptor_table();
    printk(TTY_KERNEL, "Loaded GDT at 0x%p\n", descriptor.address);
}

static void _ke_init_idt(void) {
    idt_descriptor_t descriptor = idt_init_interrupt_descriptor_table();

    isr_set_handler(DE_EXC_VECTOR, (uintptr_t)&division_by_zero_exception_handler);
    isr_set_handler(BR_EXC_VECTOR, (uintptr_t)&bound_range_exceeded_exception_handler);
    isr_set_handler(UD_EXC_VECTOR, (uintptr_t)&invalid_opcode_exception_handler);
    isr_set_handler(NM_EXC_VECTOR, (uintptr_t)&device_not_available_exception_handler);
    isr_set_handler(DF_EXC_VECTOR, (uintptr_t)&double_fault_exception_handler);
    isr_set_handler(TS_EXC_VECTOR, (uintptr_t)&invalid_tss_exception_handler);
    isr_set_handler(NP_EXC_VECTOR, (uintptr_t)&segment_not_present_exception_handler);
    isr_set_handler(SS_EXC_VECTOR, (uintptr_t)&stack_segment_fault_exception_handler);
    isr_set_handler(GP_EXC_VECTOR, (uintptr_t)&general_protection_fault_exception_handler);
    isr_set_handler(PF_EXC_VECTOR, (uintptr_t)&page_fault_exception_handler);
    isr_set_handler(MF_EXC_VECTOR, (uintptr_t)&x87_fpu_exception_handler);
    isr_set_handler(AC_EXC_VECTOR, (uintptr_t)&alignment_check_exception_handler);
    isr_set_handler(MC_EXC_VECTOR, (uintptr_t)&machine_check_exception_handler);
    isr_set_handler(XF_EXC_VECTOR, (uintptr_t)&simd_fpu_exception_handler);
    isr_set_handler(VE_EXC_VECTOR, (uintptr_t)&virtualization_exception_handler);
    isr_set_handler(SX_EXC_VECTOR, (uintptr_t)&security_exception_handler);

    isr_set_handler(DB_TRAP_VECTOR, (uintptr_t)&debug_trap_handler);
    isr_set_handler(BP_TRAP_VECTOR, (uintptr_t)&breakpoint_trap_handler);
    isr_set_handler(OF_TRAP_VECTOR, (uintptr_t)&overflow_trap_handler);

    printk(TTY_KERNEL, "Loaded IDT at 0x%p\n", descriptor.address);
}

static void _ke_init_pic(void) {
    pic_remap(32, 40);
    printk(TTY_KERNEL, "Initialized PIC\n");
}

static void _ke_init_pit(void) {
    pit_init();
    pit_set_frequency(100);
    printk(TTY_KERNEL, "System timer initialized with frequency of %dHz\n", pit_get_current_frequency());
}

static void _ke_init_kbd(void) {
    kbd_init();
    printk(TTY_KERNEL, "Looks like we got keyboard working...\n");
}

static void _ke_pit_callback(void) {
    char *text = { 0 };

    uint32_t total_ticks = pit_get_total_ticks();
    sprintf(text, "Total ticks since boot: %d", total_ticks);

    tty_set_statusbar_text(TTY_KERNEL, text);
}

static void _ke_tty_callback(tty_terminal_info_t* terminal) {
    if(terminal->index == TTY_KERNEL) {
        tty_set_colors(
            terminal->index,
            TTY_SET_COLOR_STATUSBAR | TTY_SET_COLOR_FOREGROUND |
            TTY_SET_COLOR_BACKGROUND | TTY_UPDATE_ATTRIBUTES,
            COLOR_WHITE, COLOR_BLUE
        );
    } else {
        tty_set_statusbar_text(terminal->index, " ");
    }
}

