/*
 * File name: idt.c
 * Description: x86-specific Interrupt Descriptor Table support implementation
 *
 * * * */
#include <stdint.h>

#include <hal/idt.h>
#include <hal/cpu.h>

idt_entry_t create_idt_entry(uint32_t base, uint16_t selector, uint8_t attributes) {
    idt_entry_t entry;
    entry.base_low = base & 0xFFFF;
    entry.selector = selector;
    entry.reserved = 0;
    entry.attributes = attributes;
    entry.base_high = (base >> 16) & 0xFFFF;

    return entry;
}

idt_entry_t create_kernelspace_interrupt_entry(uint32_t base) {
    return create_idt_entry(
            base,
            KERNELMODE_CODE_SELECTOR,
            DESC_TYPE_32BIT_INT | DESC_FLAGS_PRESENT);
}
