/*
 * File name: ac_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void alignment_check_exception_handler(isr_info_t *isr_info) {
    kpanic("Alignment check exception handler occured.", isr_info->eip, 0, 0);
}

