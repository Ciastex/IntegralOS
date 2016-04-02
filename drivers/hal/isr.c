/*
 * File name: isr.c
 * Description: 
 *
 * * * */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <hal/isr.h>
#include <integral/panic.h>

static isr_func_ptr handlers[256] = { 0 };

void interrupt_handler(isr_info_t *context) {
	if(handlers[context->interrupt_number]) {
		(*(handlers[context->interrupt_number]))(context);
	} else {
		kpanic("Unhandled exception occured.", context->eip, context->interrupt_number, 0);
	}
}

void isr_set_handler(uint32_t number, uintptr_t address) {
	if(number > MAX_INTERRUPT_HANDLERS) {
		kpanic("Tried to define CPU ISR index greater than 256.", number, address, 0);
	}
	handlers[number] = (isr_func_ptr)address;
}
