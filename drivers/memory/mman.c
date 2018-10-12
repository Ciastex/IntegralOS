#include <memory/mman.h>

static mman_meminfo_t _meminfo = { 0 };

mman_meminfo_t mman_initialize(multiboot_info_t* info) {
    _meminfo.lowmem_size = info->memory_lower;
    _meminfo.highmem_size = info->memory_upper;

    for(size_t i = 0; i < info->memory_map_length / sizeof(mbt_memory_map_t); i++)
    {
        mbt_memory_map_t mmap = *(info->memory_map + i);
        _meminfo.total_size += mmap.length_low;

        _meminfo.segments[i].start_addr = (uint8_t*)mmap.base_address_low;
        _meminfo.segments[i].length  = mmap.length_low;

        if(mmap.type == MULTIBOOT_MEMORY_AVAILABLE) {
            _meminfo.segments[i].available = true;
            _meminfo.usable_size += mmap.length_low;
        }
        else {
            _meminfo.segments[i].available = false;
        }
        _meminfo.seg_count++;
    }

    return _meminfo;
}
