/*
 * File name: db_th.c
 * Description: 
 *  
 * * * * */
#include <integral/trap.h>
#include <integral/panic.h>
#include <hal/isr.h>

void debug_trap_handler(isr_info_t *isr_info) {
    kpanic("Debug trap handler called.", isr_info->eip, 0, 0);
}
