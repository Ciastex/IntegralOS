/*
 * File name: gp_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void general_protection_fault_exception_handler(isr_info_t *isr_info) {
	kpanic("General protection fault exception handler called.", isr_info->eip, isr_info->error_code, 0);
}

