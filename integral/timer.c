/*
 * File name: timer.c
 * Description: 
 *  
 * * * * */
#include <io/8259a/pit.h>
#include <stdint.h>

void sleep(uint32_t ticks) {
    uint32_t target_ticks = pit_get_total_ticks() + ticks;
    while(pit_get_total_ticks() < target_ticks);
}

void sleep_seconds(uint32_t seconds) {
    uint32_t target_ticks = seconds * 100 + pit_get_total_ticks();
    while(pit_get_total_ticks() < target_ticks);
}
