/*
 * File name: br_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void bound_range_exceeded_exception_handler(isr_info_t *isr_info) {
	kpanic("Bound range exceeded exception handler called.", isr_info->eip, 0, 0);
}
