/*
 * File name: cpu.c
 * Description: x86 CPU support implementation
 *
 * * * */
#include <stdbool.h>
#include <string.h>

#include <hal/cpu.h>
#include <hal/gdt.h>
#include <hal/idt.h>
#include <hal/isr.h>
#include <hal/irq.h>

static gdt_descriptor_t gdt_descriptor;
static gdt_entry_t gdt_entries[5];

static idt_descriptor_t idt_descriptor;
static idt_entry_t idt_entries[256];

extern void low_level_load_gdt(uint32_t gdt_pointer);
extern void low_level_load_idt(uint32_t idt_pointer);

static void populate_cpu_critical_exceptions(void);
static void populate_irq_entries(void);

gdt_descriptor_t init_global_descriptor_table(void) {
    gdt_entries[0] = create_gdt_null_entry();
    gdt_entries[1] = create_kernelspace_entry(0x00000000, 0xFFFFFFFF, true);
    gdt_entries[2] = create_kernelspace_entry(0x00000000, 0xFFFFFFFF, false);
    gdt_entries[3] = create_userspace_entry(0x00000000, 0xFFFFFFFF, true);
    gdt_entries[4] = create_userspace_entry(0x00000000, 0xFFFFFFFF, false);

    gdt_descriptor.address = (uint32_t)&gdt_entries;
    gdt_descriptor.size = (sizeof(gdt_entry_t) * 5) - 1;

    low_level_load_gdt((uint32_t)&gdt_descriptor);

    return gdt_descriptor;
}

idt_descriptor_t init_interrupt_descriptor_table(void) {
    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    populate_cpu_critical_exceptions();
    populate_irq_entries();

    idt_descriptor.address = (uint32_t)&idt_entries;
    idt_descriptor.size = (sizeof(idt_entry_t) * 256) - 1;

    low_level_load_idt((uint32_t)&idt_descriptor);
    return idt_descriptor;
}

static void populate_cpu_critical_exceptions(void) {
    CPU_HW_INT_HANDLER(0);
    CPU_HW_INT_HANDLER(1);
    CPU_HW_INT_HANDLER(2);
    CPU_HW_INT_HANDLER(3);
    CPU_HW_INT_HANDLER(4);
    CPU_HW_INT_HANDLER(5);
    CPU_HW_INT_HANDLER(6);
    CPU_HW_INT_HANDLER(7);
    CPU_HW_INT_HANDLER(8);
    CPU_HW_INT_HANDLER(9);
    CPU_HW_INT_HANDLER(10);
    CPU_HW_INT_HANDLER(11);
    CPU_HW_INT_HANDLER(12);
    CPU_HW_INT_HANDLER(13);
    CPU_HW_INT_HANDLER(14);
    CPU_HW_INT_HANDLER(15);
    CPU_HW_INT_HANDLER(16);
    CPU_HW_INT_HANDLER(17);
    CPU_HW_INT_HANDLER(18);
    CPU_HW_INT_HANDLER(19);
    CPU_HW_INT_HANDLER(20);
    CPU_HW_INT_HANDLER(21);
    CPU_HW_INT_HANDLER(22);
    CPU_HW_INT_HANDLER(23);
    CPU_HW_INT_HANDLER(24);
    CPU_HW_INT_HANDLER(25);
    CPU_HW_INT_HANDLER(26);
    CPU_HW_INT_HANDLER(27);
    CPU_HW_INT_HANDLER(28);
    CPU_HW_INT_HANDLER(29);
    CPU_HW_INT_HANDLER(30);
    CPU_HW_INT_HANDLER(31);
}

static void populate_irq_entries(void) {
    IRQ_INT_HANDLER(32);
    IRQ_INT_HANDLER(33);
    IRQ_INT_HANDLER(34);
    IRQ_INT_HANDLER(35);
    IRQ_INT_HANDLER(36);
    IRQ_INT_HANDLER(37);
    IRQ_INT_HANDLER(38);
    IRQ_INT_HANDLER(39);
    IRQ_INT_HANDLER(40);
    IRQ_INT_HANDLER(41);
    IRQ_INT_HANDLER(42);
    IRQ_INT_HANDLER(43);
    IRQ_INT_HANDLER(44);
    IRQ_INT_HANDLER(45);
    IRQ_INT_HANDLER(46);
}
