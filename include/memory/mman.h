#ifndef __INCLUDE_MMAN_H
#define __INCLUDE_MMAN_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <boot/multiboot.h>
#include <integral/kernel.h>

typedef struct mman_memseg {
    uint8_t* start_addr;
    uint32_t length;
    bool available;
} mman_memseg_t;

typedef struct mman_meminfo {
    uint32_t lowmem_size;
    uint32_t highmem_size;
    uint32_t usable_size;
    uint32_t total_size;
    size_t seg_count;
    mman_memseg_t segments[32];
} mman_meminfo_t;

mman_meminfo_t mman_initialize(multiboot_info_t* info);

#endif //__INCLUDE_MMAN_H
