/*
 * File name: np_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void segment_not_present_exception_handler(isr_info_t *isr_info) {
    kpanic("Segment not present exception handler called.", isr_info->eip, isr_info->error_code, 0);
}

