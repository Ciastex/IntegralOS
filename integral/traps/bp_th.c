/*
 * File name: bp_th.c
 * Description: 
 *  
 * * * * */
#include <integral/trap.h>
#include <integral/panic.h>
#include <hal/isr.h>

void breakpoint_trap_handler(isr_info_t *isr_info) {
    kpanic("Breakpoint trap handler called.", isr_info->eip, 0, 0);
}

