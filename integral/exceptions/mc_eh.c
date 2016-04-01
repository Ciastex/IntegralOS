/*
 * File name: mc_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void machine_check_exception_handler(isr_info_t *isr_info) {
	kpanic("Machine check exception handler called.", isr_info->eip, 0, 0);
}
