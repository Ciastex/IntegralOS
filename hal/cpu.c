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

static gdt_descriptor_t gdt_descriptor;
static gdt_entry_t gdt_entries[5];

static idt_descriptor_t idt_descriptor;
static idt_entry_t idt_entries[256];

extern void low_level_load_gdt(uint32_t gdt_pointer);
extern void low_level_load_idt(uint32_t idt_pointer);

static void populate_cpu_critical_exceptions(void);

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

    idt_descriptor.address = (uint32_t)&idt_entries;
    idt_descriptor.size = (sizeof(idt_entry_t) * 256) - 1;

    low_level_load_idt((uint32_t)&idt_descriptor);
    return idt_descriptor;
}

static void populate_cpu_critical_exceptions(void) {
    idt_entries[0] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_0);
    idt_entries[1] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_1);
    idt_entries[2] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_2);
    idt_entries[3] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_3);
    idt_entries[4] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_4);
    idt_entries[5] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_5);
    idt_entries[6] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_6);
    idt_entries[7] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_7);
    idt_entries[8] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_8);
    idt_entries[9] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_9);
    idt_entries[10] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_10);
    idt_entries[11] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_11);
    idt_entries[12] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_12);
    idt_entries[13] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_13);
    idt_entries[14] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_14);
    idt_entries[15] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_15);
    idt_entries[16] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_16);
    idt_entries[17] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_17);
    idt_entries[18] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_18);
    idt_entries[19] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_19);
    idt_entries[20] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_20);
    idt_entries[21] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_21);
    idt_entries[22] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_22);
    idt_entries[23] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_23);
    idt_entries[24] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_24);
    idt_entries[25] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_25);
    idt_entries[26] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_26);
    idt_entries[27] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_27);
    idt_entries[28] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_28);
    idt_entries[29] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_29);
    idt_entries[30] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_30);
    idt_entries[31] = create_kernelspace_interrupt_entry((uint32_t)&isr_handler_31);
}
