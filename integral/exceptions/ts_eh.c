/*
 * File name: ts_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void invalid_tss_exception_handler(isr_info_t *isr_info) {
	kpanic("Invalid TSS exception handler called.", isr_info->eip, isr_info->error_code, 0);
}

