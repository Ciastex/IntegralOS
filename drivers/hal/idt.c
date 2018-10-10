/*
 * File name: idt.c
 * Description: x86-specific Interrupt Descriptor Table support implementation
 *
 * * * */
#include <stdint.h>
#include <string.h>

#include <hal/idt.h>
#include <hal/gdt.h>
#include <hal/isr.h>
#include <hal/irq.h>

extern void idt_load_ll(idt_descriptor_t* idt);

static void populate_cpu_critical_exceptions(void);
static void populate_irq_entries(void);

static idt_descriptor_t idt_descriptor;
static idt_entry_t idt_entries[256];

idt_entry_t idt_create_entry(uint32_t base, uint16_t selector, uint8_t attributes) {
    idt_entry_t entry;
    entry.base_low = base & 0xFFFF;
    entry.selector = selector;
    entry.reserved = 0;
    entry.attributes = attributes;
    entry.base_high = (base >> 16) & 0xFFFF;

    return entry;
}

idt_entry_t idt_create_kernelspace_interrupt_entry(uint32_t base) {
    return idt_create_entry(
            base,
            KERNELMODE_CODE_SELECTOR,
            DESC_TYPE_32BIT_INT | DESC_FLAGS_PRESENT);
}

idt_descriptor_t idt_init_interrupt_descriptor_table(void) {
    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    populate_cpu_critical_exceptions();
    populate_irq_entries();

    idt_descriptor.address = (uint32_t)&idt_entries;
    idt_descriptor.size = (sizeof(idt_entry_t) * 256) - 1;

    idt_load_ll(&idt_descriptor);
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
