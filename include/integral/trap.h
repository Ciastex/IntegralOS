/*
 * File name: trap_handlers.h
 * Description: 
 *  
 * * * * */

#ifndef INCLUDE_INTEGRAL_INTERRUPT_TRAP_HANDLERS_H_
#define INCLUDE_INTEGRAL_INTERRUPT_TRAP_HANDLERS_H_

#include <hal/isr.h>

#define DB_TRAP_VECTOR 1
#define BP_TRAP_VECTOR 3
#define OF_TRAP_VECTOR 4

void debug_trap_handler(isr_info_t *isr_info);
void breakpoint_trap_handler(isr_info_t *isr_info);
void overflow_trap_handler(isr_info_t *isr_info);

#endif /* INCLUDE_INTEGRAL_INTERRUPT_TRAP_HANDLERS_H_ */
