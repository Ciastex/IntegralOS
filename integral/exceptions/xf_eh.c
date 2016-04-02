/*
 * File name: xf_eh.c
 * Description: 
 *  
 * * * * */
#include <integral/exception.h>
#include <integral/panic.h>
#include <hal/isr.h>

void simd_fpu_exception_handler(isr_info_t *isr_info) {
    kpanic("SIMD FPU exception handler called.", isr_info->eip, 0, 0);
}

