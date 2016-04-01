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

#include <hal/idt.h>
#include <hal/gdt.h>
#include <hal/irq.h>

#include <display/vga.h>
#include <io/port_io.h>
#include <io/8259a/pic.h>

static void print_welcome_screen(void);
static void init_gdt(void);
static void init_idt(void);
static void init_pic(void);

void kernel_init(multiboot_info_t *multiboot_info, uint32_t bootloader_magic) {
    init_terminal();

    if(bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kpanic("Not loaded by a multiboot-compliant bootloader.", bootloader_magic, 0, 0);
    } else {
        print_welcome_screen();

        init_gdt();
        init_idt();
        init_pic();

        asm volatile("int $1");

        for(;;);
    }
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

    set_interrupt_handler(DE_EXC_VECTOR, (uint32_t)&division_by_zero_exception_handler);
    set_interrupt_handler(BR_EXC_VECTOR, (uint32_t)&bound_range_exceeded_exception_handler);
    set_interrupt_handler(UD_EXC_VECTOR, (uint32_t)&invalid_opcode_exception_handler);
    set_interrupt_handler(NM_EXC_VECTOR, (uint32_t)&device_not_available_exception_handler);
    set_interrupt_handler(DF_EXC_VECTOR, (uint32_t)&double_fault_exception_handler);
    set_interrupt_handler(TS_EXC_VECTOR, (uint32_t)&invalid_tss_exception_handler);
    set_interrupt_handler(NP_EXC_VECTOR, (uint32_t)&segment_not_present_exception_handler);
    set_interrupt_handler(SS_EXC_VECTOR, (uint32_t)&stack_segment_fault_exception_handler);
    set_interrupt_handler(GP_EXC_VECTOR, (uint32_t)&general_protection_fault_exception_handler);
    set_interrupt_handler(PF_EXC_VECTOR, (uint32_t)&page_fault_exception_handler);
    set_interrupt_handler(MF_EXC_VECTOR, (uint32_t)&x87_fpu_exception_handler);
    set_interrupt_handler(AC_EXC_VECTOR, (uint32_t)&alignment_check_exception_handler);
    set_interrupt_handler(MC_EXC_VECTOR, (uint32_t)&machine_check_exception_handler);
    set_interrupt_handler(XF_EXC_VECTOR, (uint32_t)&simd_fpu_exception_handler);
    set_interrupt_handler(VE_EXC_VECTOR, (uint32_t)&virtualization_exception_handler);
    set_interrupt_handler(SX_EXC_VECTOR, (uint32_t)&security_exception_handler);

    set_interrupt_handler(DB_TRAP_VECTOR, (uint32_t)&debug_trap_handler);
    set_interrupt_handler(BP_TRAP_VECTOR, (uint32_t)&breakpoint_trap_handler);
    set_interrupt_handler(OF_TRAP_VECTOR, (uint32_t)&overflow_trap_handler);

    printf("\\[2Loaded @\\X 0x%08X\n", descriptor.address);
}

static void init_pic(void) {
    printf("Initializing 8259A PIC... ");
    pic_remap(32, 40);
    printf("\\[2OK\\X\nIRQ_VEC_START_\\[AMASTER\\X = 32\nIRQ_VEC_START_\\[ASLAVE\\X = 40\n");
}
