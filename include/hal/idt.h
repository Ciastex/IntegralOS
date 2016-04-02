/*
 * File name: idt.h
 * Description: x86-specific Interrupt Descriptor Table interface header.
 *
 * * * */

#ifndef IDT_H_
#define IDT_H_

/*
 * Attribute structure:
 *
 *   7                           0
 * +---+---+---+---+---+---+---+---+
 * | P |  DPL  | S |    GateType   |
 * +---+---+---+---+---+---+---+---+
 *
 */

#define DESC_TYPE_32BIT_TASK 0x0005 // 0b0101
#define DESC_TYPE_16BIT_INT  0x0006 // 0b0110
#define DESC_TYPE_16BIT_TRAP 0x0007 // 0b0111
#define DESC_TYPE_32BIT_INT  0x000E // 0b1110
#define DESC_TYPE_32BIT_TRAP 0x000F // 0b1111

#define DESC_FLAGS_NOT_INT 0x0010
#define DESC_FLAGS_RING1   0x0020
#define DESC_FLAGS_RING2   0x0040
#define DESC_FLAGS_RING3   0x0060
#define DESC_FLAGS_PRESENT 0x0080

typedef struct idt_descriptor {
    uint16_t size;
    uint32_t address;
} __attribute((packed)) idt_descriptor_t;

typedef struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t base_high;
} idt_entry_t;

idt_entry_t idt_create_entry(uint32_t base, uint16_t selector, uint8_t attributes);
idt_entry_t idt_create_kernelspace_interrupt_entry(uint32_t base);
idt_descriptor_t idt_init_interrupt_descriptor_table(void);

#endif /* IDT_H_ */
