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
		kpanic("Unhandled exception occured.");
	}
}

void set_interrupt_handler(uint32_t number, uint32_t address) {
	if(number > MAX_INTERRUPT_HANDLERS) {
		kpanic("Tried to initialize CPU interrupt handler greater than 256");
	}
	handlers[number] = (isr_func_ptr)address;
}
