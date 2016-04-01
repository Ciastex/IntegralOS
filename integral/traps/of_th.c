/*
 * File name: of_th.c
 * Description: 
 *  
 * * * * */
#include <integral/trap.h>
#include <integral/panic.h>
#include <hal/isr.h>

void overflow_trap_handler(isr_info_t *isr_info) {
	kpanic("Overflow trap handler called.", isr_info->eip, isr_info->eflags, 0);
}
