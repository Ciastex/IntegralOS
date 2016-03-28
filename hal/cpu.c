/*
 * File name: cpu.c
 * Description: x86 CPU support implementation
 *
 * * * */
#include <stdbool.h>
#include <string.h>

#include <hal/cpu.h>
#include <hal/gdt.h>

static gdt_descriptor_t gdt_descriptor;
static gdt_entry_t gdt_entries[5];

extern void low_level_load_gdt(uint32_t gdt_pointer);

gdt_descriptor_t init_global_descriptor_table(void) {
    gdt_entries[0] = create_gdt_null_entry();
    gdt_entries[1] = create_kernelspace_entry(0x00000000, 0xFFFFFFFF, true);
    gdt_entries[2] = create_kernelspace_entry(0x00000000, 0xFFFFFFFF, false);
    gdt_entries[3] = create_userspace_entry(0x00000000, 0xFFFFFFFF, true);
    gdt_entries[4] = create_userspace_entry(0x00000000, 0xFFFFFFFF, false);

    memset(&gdt_descriptor, 0, sizeof(gdt_descriptor_t));
    gdt_descriptor.address = (uint32_t)&gdt_entries;
    gdt_descriptor.size = (sizeof(gdt_entry_t) * 5) - 1;

    low_level_load_gdt((uint32_t)&gdt_descriptor);

    return gdt_descriptor;
}
