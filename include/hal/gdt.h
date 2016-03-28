/*
 * File name: gdt.h
 * Description: x86-specific GDT header.
 *
 * * * */

#ifndef GDT_H_
#define GDT_H_

#include <stdbool.h>
#include <stdint.h>

#define ACCESS_DESC_ACCESSED         0x0001
#define ACCESS_DESC_READWRITE        0x0002 // 1 - allowed: read for code; write for data; else restrict
#define ACCESS_DESC_DIRECTION        0x0004 // 0 - segment grows up; 1 - segment grows down
#define ACCESS_DESC_EXECUTE          0x0008 // 1 - code selector (can execute); 0 - data selector (can't execute)
#define ACCESS_DESC_RESERVED         0x0010 // should always be 1
#define ACCESS_DESC_PRIVILEGE_RING_1 0x0020 // ]
#define ACCESS_DESC_PRIVILEGE_RING_2 0x0040 // > RING_0 = all masks are 0
#define ACCESS_DESC_PRIVILEGE_RING_3 0x0060 // ]
#define ACCESS_DESC_PRESENT          0x0080 // 1 - valid descriptor; 0 - invalid descriptor

#define FLAGS_SIZE_32BIT             0x0040 // 0 - defines 16-bit protected mode; 1 - defines 32-bit protected mode
#define FLAGS_GRANULARITY_4K         0x0080 // 0: 1 byte granularity; 1: 4 kilobyte granularity

typedef struct gdt_descriptor {
    uint16_t size;
    uint32_t address;
} __attribute((packed)) gdt_descriptor_t;

typedef struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute((packed)) gdt_entry_t;

gdt_entry_t create_gdt_entry(uint32_t base_addr, uint32_t limit, uint8_t access, uint8_t granularity);
gdt_entry_t create_gdt_null_entry(void);
gdt_entry_t create_kernelspace_entry(uint32_t base_addr, uint32_t limit, bool is_code);
gdt_entry_t create_userspace_entry(uint32_t base_addr, uint32_t limit, bool is_code);

#endif /* GDT_H_ */
