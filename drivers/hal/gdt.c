/*
 * File name: gdt.c
 * Description: x86-specific GDT implementation
 *
 * * * */

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <hal/gdt.h>

extern void gdt_load_ll(gdt_descriptor_t* descriptor);

static gdt_descriptor_t gdt_descriptor;
static gdt_entry_t gdt_entries[5];

gdt_entry_t gdt_create_entry(uint32_t base_addr, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entry_t entry;
    memset(&entry, 0, sizeof(gdt_entry_t));

    entry.base_low = base_addr & 0xFFFF;
    entry.base_mid = (base_addr >> 16) & 0xFF;
    entry.base_high = (base_addr >> 24) & 0xFF;

    entry.limit_low = limit & 0xFFFF;
    entry.access = access;
    entry.granularity = (limit >> 16) & 0x0F;
    entry.granularity |= granularity & 0xF0;
    return entry;
}

gdt_entry_t gdt_create_null_entry(void) {
    return gdt_create_entry(0, 0, 0, 0);
}

gdt_entry_t gdt_create_kernelspace_entry(uint32_t base_addr, uint32_t limit, bool is_code) {
    uint16_t can_execute = 0;

    if(is_code) {
        can_execute = ACCESS_DESC_EXECUTE;
    }

    return gdt_create_entry(
        base_addr,
        limit,
        ACCESS_DESC_READWRITE | can_execute | ACCESS_DESC_RESERVED | ACCESS_DESC_PRESENT,
        FLAGS_SIZE_32BIT | FLAGS_GRANULARITY_4K | 0x0F
    );
}

gdt_entry_t gdt_create_userspace_entry(uint32_t base_addr, uint32_t limit, bool is_code) {
    uint16_t can_execute = 0;

    if(is_code) {
        can_execute = ACCESS_DESC_EXECUTE;
    }

    return gdt_create_entry(
        base_addr,
        limit,
        ACCESS_DESC_READWRITE | can_execute | ACCESS_DESC_RESERVED | ACCESS_DESC_PRESENT | ACCESS_DESC_PRIVILEGE_RING_3,
        FLAGS_SIZE_32BIT | FLAGS_GRANULARITY_4K | 0x0F
    );
}

gdt_descriptor_t gdt_init_global_descriptor_table(void) {
    gdt_entries[0] = gdt_create_null_entry();
    gdt_entries[1] = gdt_create_kernelspace_entry(0x00000000, 0xFFFFF, true);
    gdt_entries[2] = gdt_create_kernelspace_entry(0x00000000, 0xFFFFF, false);
    gdt_entries[3] = gdt_create_userspace_entry(0x00000000, 0xFFFFF, true);
    gdt_entries[4] = gdt_create_userspace_entry(0x00000000, 0xFFFFF, false);

    gdt_descriptor.address = (gdt_entry_t*)&gdt_entries;
    gdt_descriptor.size = (sizeof(gdt_entry_t) * 5) - 1;

    gdt_load_ll(&gdt_descriptor);

    return gdt_descriptor;
}
