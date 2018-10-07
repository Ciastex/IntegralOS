/*
 * File name: multiboot.h
 * Description: The Multiboot standard header.
 *
 * * * */

#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

typedef struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
} multiboot_header_t;

typedef struct aout_symbol_table {
    uint32_t tab_size;
    uint32_t str_size;
    uint32_t address;
    uint32_t reserved;
} aout_symbol_table_t;

typedef struct elf_section_header_table {
    uint32_t num;
    uint32_t size;
    uint32_t address;
    uint32_t shndx;
} elf_section_header_table_t;

typedef struct multiboot_module {
    uint32_t module_start;
    uint32_t module_end;
    uint32_t string;
    uint32_t reserved;
} multiboot_module_t;

typedef struct memory_map {
    uint32_t size;
    uint32_t base_address_low;
    uint32_t base_address_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} memory_map_t;

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t memory_lower;
    uint32_t memory_upper;
    uint32_t boot_device;
    uint32_t command_line;
    uint32_t modules_count;
    multiboot_module_t* modules_first;
    union {
        aout_symbol_table_t aout_symbols;
        elf_section_header_table_t elf_section_table;
    } additional_info;
    uint32_t memory_map_length;
    memory_map_t* memory_map;
} multiboot_info_t;



#endif /* MULTIBOOT_H_ */
