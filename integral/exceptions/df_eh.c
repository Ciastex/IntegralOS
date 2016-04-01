/*
 * File name: df_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void double_fault_exception_handler(isr_info_t *isr_info) {
	kpanic("Double fault exception handler called.", isr_info->error_code, 0, 0);
}
