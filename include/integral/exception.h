/*
 * File name: handlers.h
 * Description: 
 *  
 * * * * */

#ifndef INCLUDE_INTEGRAL_HANDLERS_H_
#define INCLUDE_INTEGRAL_HANDLERS_H_

#include <hal/isr.h>

#define DE_EXC_VECTOR 0
#define NMI_EXC_VECTOR 2
#define BR_EXC_VECTOR 5
#define UD_EXC_VECTOR 6
#define NM_EXC_VECTOR 7
#define DF_EXC_VECTOR 8
#define TS_EXC_VECTOR 10
#define NP_EXC_VECTOR 11
#define SS_EXC_VECTOR 12
#define GP_EXC_VECTOR 13
#define PF_EXC_VECTOR 14
#define MF_EXC_VECTOR 16
#define AC_EXC_VECTOR 17
#define MC_EXC_VECTOR 18
#define XF_EXC_VECTOR 19
#define VE_EXC_VECTOR 20
#define SX_EXC_VECTOR 30

void division_by_zero_exception_handler(isr_info_t *isr_info);
void non_maskable_interrupt_handler(isr_info_t *isr_info);
void bound_range_exceeded_exception_handler(isr_info_t *isr_info);
void invalid_opcode_exception_handler(isr_info_t *isr_info);
void device_not_available_exception_handler(isr_info_t *isr_info);
void double_fault_exception_handler(isr_info_t *isr_info);
void invalid_tss_exception_handler(isr_info_t *isr_info);
void segment_not_present_exception_handler(isr_info_t *isr_info);
void stack_segment_fault_exception_handler(isr_info_t *isr_info);
void general_protection_fault_exception_handler(isr_info_t *isr_info);
void page_fault_exception_handler(isr_info_t *isr_info);
void x87_fpu_exception_handler(isr_info_t *isr_info);
void alignment_check_exception_handler(isr_info_t *isr_info);
void machine_check_exception_handler(isr_info_t *isr_info);
void simd_fpu_exception_handler(isr_info_t *isr_info);
void virtualization_exception_handler(isr_info_t *isr_info);
void security_exception_handler(isr_info_t *isr_info);


#endif /* INCLUDE_INTEGRAL_HANDLERS_H_ */
