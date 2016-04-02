/*
 * File name: nm_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void device_not_available_exception_handler(isr_info_t *isr_info) {
    kpanic("Device not available exception handler called.", isr_info->eip, 0, 0);
}
