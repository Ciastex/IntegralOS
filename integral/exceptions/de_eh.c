/*
 * File name: div_by_zero_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void division_by_zero_exception_handler(isr_info_t *isr_info) {
	kpanic("Division exception occured.", isr_info->eip, 0, 0);
}
