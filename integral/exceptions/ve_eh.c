/*
 * File name: ve_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void virtualization_exception_handler(isr_info_t *isr_info) {
	kpanic("Virtualization exception handler called.", isr_info->eip, 0, 0);
}

