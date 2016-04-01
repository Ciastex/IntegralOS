/*
 * File name: mf_eh.c
 * Description: 
 *  
 * * * * */
#include <stdlib.h>

#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void x87_fpu_exception_handler(isr_info_t *isr_info) {
	uint32_t cr0 = 0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));

	if(IS_BIT_SET(cr0, 5)) {
		kpanic("X87 FPU exception", isr_info->eip, 0, 0);
	}

	//todo x87 status word register
}

