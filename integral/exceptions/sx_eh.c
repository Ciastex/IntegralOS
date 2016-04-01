/*
 * File name: sx_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void security_exception_handler(isr_info_t *isr_info) {
	kpanic("Security exception handler called.", isr_info->eip, isr_info->error_code, 0);
}
