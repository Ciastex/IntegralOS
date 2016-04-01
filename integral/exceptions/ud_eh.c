/*
 * File name: ud_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void invalid_opcode_exception_handler(isr_info_t *isr_info) {
	kpanic("Invalid opcode exception handler called.", isr_info->eip, 0, 0);
}

