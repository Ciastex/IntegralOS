#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

#include <stdlib.h>

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

#define MBTFLAG_MEM_VALID(strct)         IS_BIT_SET(strct->flags, 0)
#define MBTFLAG_BOOTDEV_VALID(strct)     IS_BIT_SET(strct->flags, 1)
#define MBTFLAG_CMDLINE_VALID(strct)     IS_BIT_SET(strct->flags, 2)
#define MBTFLAG_MODULES_VALID(strct)     IS_BIT_SET(strct->flags, 3)
#define MBTFLAG_SYMBOLS_VALID(strct)     ((IS_BIT_SET(strct->flags, 4)) || (IS_BIT_SET(strct->flags, 5)))
#define MBTFLAG_MEMMAP_VALID(strct)      IS_BIT_SET(strct->flags, 6)
#define MBTFLAG_DRIVES_VALID(strct)      IS_BIT_SET(strct->flags, 7)
#define MBTFLAG_BIOSCONF_VALID(strct)    IS_BIT_SET(strct->flags, 8)
#define MBTFLAG_LDRNAME_VALID(strct)     IS_BIT_SET(strct->flags, 9)
#define MBTFLAG_APMTABLE_VALID(strct)    IS_BIT_SET(strct->flags, 10)
#define MBTFLAG_VBECTINFO_VALID(strct)   IS_BIT_SET(strct->flags, 11)
#define MBTFLAG_FRAMEBUFFER_VALID(strct) IS_BIT_SET(strct->flags, 12)

typedef uint8_t mbt_bios_featurebyte;

typedef struct mbt_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
} mbt_header_t;

typedef struct mbt_aout_symbol_table {
    uint32_t tab_size;
    uint32_t str_size;
    uint32_t address;
    uint32_t reserved;
} mbt_aout_symbol_table_t;

typedef struct mbt_elf_section_header_table {
    uint32_t num;
    uint32_t size;
    uint32_t address;
    uint32_t shndx;
} mbt_elf_section_header_table_t;

typedef struct mbt_module {
    uint32_t module_start;
    uint32_t module_end;
    uint32_t string;
    uint32_t reserved;
} mbt_module_t;

typedef struct mbt_memory_map {
    uint32_t size;
    uint32_t base_address_low;
    uint32_t base_address_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} mbt_memory_map_t;

typedef struct mbt_bios_config {
    uint16_t size;
    uint8_t model;
    uint8_t submodel;
    uint8_t bios_revision;
    mbt_bios_featurebyte feature_byte_1;
    mbt_bios_featurebyte feature_byte_2;
    mbt_bios_featurebyte feature_byte_3;
    mbt_bios_featurebyte feature_byte_4;
    mbt_bios_featurebyte feature_byte_5;
} mbt_bios_config_t;

typedef struct mbt_apm_info {
    uint16_t version;
    uint16_t cseg;
    uint16_t offset;
    uint16_t cseg16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t cseg_length;
    uint16_t cseg16_length;
    uint16_t dseg_length;
} mbt_apm_info_t;

typedef struct mbt_vbe_control_info {
    char signature[4];
    uint16_t revision;
    char* oem_name;
    uint32_t capabilities;
    uint32_t video_modes;
    uint16_t video_memory_blocks;
    uint16_t software_rev;
    char* vendor_string;
    char* product_name;
    char* product_ver;
    char reserved[222];
    char oem_data[256];
} __attribute__((packed)) mbt_vbe_control_info_t;

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t memory_lower;
    uint32_t memory_upper;
    uint32_t boot_device;
    char* command_line;
    uint32_t modules_count;
    mbt_module_t* modules_first;
    union {
        mbt_aout_symbol_table_t aout_symbols;
        mbt_elf_section_header_table_t elf_section_table;
    } additional_info;
    uint32_t memory_map_length;
    mbt_memory_map_t* memory_map;
    uint32_t drives_length;
    uint32_t drives_addr;
    mbt_bios_config_t* bios_config;
    char* bootloader_name;
    mbt_apm_info_t* apm_info;
    mbt_vbe_control_info_t* vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __attribute__((packed)) multiboot_info_t;



#endif /* MULTIBOOT_H_ */
