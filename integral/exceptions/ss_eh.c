/*
 * File name: ss_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void stack_segment_fault_exception_handler(isr_info_t *isr_info) {
    kpanic("Stack-segment fault exception handler called.", isr_info->eip, isr_info->error_code, 0);
}

