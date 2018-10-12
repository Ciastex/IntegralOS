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

static isr_handler_t* handlers[ISR_MAX_HANDLERS] = { 0 };

void isr_set_handler(uint32_t number, isr_handler_t* handler) {
    if(number >= ISR_MAX_HANDLERS - 1) {
        kpanic("Tried to define CPU ISR index greater than 255.", number, (uint32_t)handler, 0);
    }
    handlers[number] = handler;
}


void interrupt_handler(isr_info_t *context) {
    if(handlers[context->interrupt_number]) {
        (*(handlers[context->interrupt_number]))(context);
    } else {
        kpanic("Unhandled exception occured.", context->eip, context->interrupt_number, 0);
    }
}
