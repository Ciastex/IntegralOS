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

#include <memory/mman.h>

#include <io/port_io.h>
#include <io/8259a/pic.h>
#include <io/8259a/pit.h>
#include <io/8042/ps2.h>
#include <io/keyboard/keyboard.h>

static void _ke_print_welcome_screen(void);
static void _ke_init_gdt(void);
static void _ke_init_idt(void);
static void _ke_init_pic(void);
static void _ke_init_pit(void);
static void _ke_init_ps2(void);
static void _ke_print_mbt(multiboot_info_t* info);
static void _ke_pit_callback(void);
static void _ke_tty_callback(tty_terminal_info_t* terminal);
static void _ke_tty_switch_callback(tty_terminal_info_t* current, tty_terminal_info_t* prev);

void kernel_init(multiboot_info_t* multiboot_info, uint32_t bootloader_magic) {
    tty_init_terminals(_ke_tty_callback);
    tty_set_on_switch_callback(&_ke_tty_switch_callback);

    if(bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kpanic("Not loaded by a multiboot-compliant bootloader.", bootloader_magic, 0, 0);
    } else {
        _ke_init_gdt();
        _ke_init_idt();
        _ke_init_pic();
        _ke_init_pit();
        _ke_init_ps2();
        _ke_print_mbt(multiboot_info);
        kbd_initialize();

        pit_set_callback(&_ke_pit_callback);
        _ke_print_welcome_screen();

        while(true) {
            printf("$> ");
            char buffer[255] = { 0 };
            tty_read_line(0, buffer, 255);

            if(!strcmp(buffer, "test")) {
                printf("yay!\n");
            }
            tty_statprintf(0, "last input: %s", buffer);
        }

        for(;;);
    }
}

static void _ke_print_welcome_screen(void) {
    printf("\n\\[AIntegral OS kernel\\X v%s\n", INTEGRAL_VERSION);
    for(size_t i = 0; i < 40; i++) {
        printf("-");
    }
    printf("\n");
    printf("Preparing the operating system environment...\n");
    printf("Use keys F1-F8 to switch virtual terminals.\n");
}

static void _ke_init_gdt(void) {
    gdt_descriptor_t descriptor = gdt_init_global_descriptor_table();
    printf("_ke_init_gdt: 0x%p\n", descriptor.address);
}

static void _ke_init_idt(void) {
    idt_descriptor_t descriptor = idt_init_interrupt_descriptor_table();

    isr_set_handler(DE_EXC_VECTOR, &division_by_zero_exception_handler);
    isr_set_handler(BR_EXC_VECTOR, &bound_range_exceeded_exception_handler);
    isr_set_handler(UD_EXC_VECTOR, &invalid_opcode_exception_handler);
    isr_set_handler(NM_EXC_VECTOR, &device_not_available_exception_handler);
    isr_set_handler(DF_EXC_VECTOR, &double_fault_exception_handler);
    isr_set_handler(TS_EXC_VECTOR, &invalid_tss_exception_handler);
    isr_set_handler(NP_EXC_VECTOR, &segment_not_present_exception_handler);
    isr_set_handler(SS_EXC_VECTOR, &stack_segment_fault_exception_handler);
    isr_set_handler(GP_EXC_VECTOR, &general_protection_fault_exception_handler);
    isr_set_handler(PF_EXC_VECTOR, &page_fault_exception_handler);
    isr_set_handler(MF_EXC_VECTOR, &x87_fpu_exception_handler);
    isr_set_handler(AC_EXC_VECTOR, &alignment_check_exception_handler);
    isr_set_handler(MC_EXC_VECTOR, &machine_check_exception_handler);
    isr_set_handler(XF_EXC_VECTOR, &simd_fpu_exception_handler);
    isr_set_handler(VE_EXC_VECTOR, &virtualization_exception_handler);
    isr_set_handler(SX_EXC_VECTOR, &security_exception_handler);

    isr_set_handler(DB_TRAP_VECTOR, &debug_trap_handler);
    isr_set_handler(BP_TRAP_VECTOR, &breakpoint_trap_handler);
    isr_set_handler(OF_TRAP_VECTOR, &overflow_trap_handler);

    printf("_ke_init_idt: 0x%p\n", descriptor.address);
}

static void _ke_init_pic(void) {
    pic_remap(32, 40);
    printf("_ke_init_pic: IRQs remapped\n");
}

static void _ke_init_pit(void) {
    pit_init();
    pit_set_frequency(100);
    printf("_ke_init_pit: initialized timer. freq: %dHz\n", pit_get_current_frequency());
}

static void _ke_init_ps2(void) {
    printf("_ke_init_ps2: start\n");
    ps2_initialize();
    printf("_ke_init_ps2: done\n");
}

static void _ke_print_mbt(multiboot_info_t* info) {
    if(MBTFLAG_MEM_VALID(info)) {
        printf("_ke_print_mbt: got low/high memory values: low: %dK, high: %dK\n", info->memory_lower, info->memory_upper);
    }

    if(MBTFLAG_BOOTDEV_VALID(info)) {
        printf("_ke_print_mbt: got bootdev number: %d\n", info->boot_device);
    }

    if(MBTFLAG_CMDLINE_VALID(info)) {
        printf("_ke_print_mbt: got cmdline: %s\n", info->command_line);
    }

    if(MBTFLAG_SYMBOLS_VALID(info)) {
        printf("_ke_print_mbt: got symbols: 0x%p\n", info->additional_info);
    }

    if(MBTFLAG_MEMMAP_VALID(info)) {
        printf("_ke_print_mbt: got valid memory map: 0x%p, %d bytes long\n", info->memory_map, info->memory_map_length);
    }

    if(MBTFLAG_DRIVES_VALID(info)) {
        printf("_ke_print_mbt: got some drive info: %0x%p, %d bytes long\n", info->drives_addr, info->drives_length);
    }

    if(MBTFLAG_BIOSCONF_VALID(info)) {
        printf("_ke_print_mbt: bios config table detected: 0x%p\n", info->bios_config);
    }

    if(MBTFLAG_LDRNAME_VALID(info)) {
        printf("_ke_print_mbt: we know what loaded us: %s\n", info->bootloader_name);
    }

    if(MBTFLAG_APMTABLE_VALID(info)) {
        printf("_ke_print_mbt: apm table present: 0x%p", info->apm_info);
    }

    if(MBTFLAG_VBECTINFO_VALID(info)) {
        printf("_ke_print_mbt: got vbe params: 0x%p", info->vbe_control_info);
        printf("  vesa string: %s", info->vbe_control_info->signature);
        printf("  vesa oem: %s", info->vbe_control_info->oem_name);
    }

    if(MBTFLAG_FRAMEBUFFER_VALID(info)) {
        printf("_ke_print_mbt: got framebuffer: 0x%p");
    }
}

static void _ke_pit_callback(void) {
    tty_statprintf(TTY_KERNEL, "Total ticks since boot: %d", pit_get_total_ticks());
}

static void _ke_tty_callback(tty_terminal_info_t* terminal) {
    if(terminal->index == TTY_KERNEL) {
        tty_set_colors(
            terminal->index,
            TTY_SET_COLOR_STATUSBAR | TTY_SET_COLOR_FOREGROUND |
            TTY_SET_COLOR_BACKGROUND | TTY_UPDATE_ATTRIBUTES,
            COLOR_WHITE, COLOR_BLUE
        );
    } else if(terminal->index == 0) {
        tty_set_colors(
            terminal->index,
            TTY_SET_COLOR_STATUSBAR | TTY_SET_COLOR_FOREGROUND |
            TTY_SET_COLOR_BACKGROUND | TTY_UPDATE_ATTRIBUTES,
            COLOR_BLACK, COLOR_GREEN
        );
    } else {
        tty_statprintf(terminal->index, "Integral OS Kernel | terminal #%d", terminal->index);
    }
}

static void _ke_tty_switch_callback(tty_terminal_info_t* current, tty_terminal_info_t* prev) {

}

