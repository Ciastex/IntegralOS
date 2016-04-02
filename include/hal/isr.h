/*
 * File name: isr.h
 * Description: 
 *
 * * * */

#ifndef ISR_H_
#define ISR_H_

#include <stdint.h>

#define EXTERN_ISR_HANDLER_NAME(x) isr_handler_ ## x
#define EXTERN_ISR_HANDLER(x) extern void EXTERN_ISR_HANDLER_NAME(x)(void)
#define CPU_HW_INT_HANDLER(x) idt_entries[x] = idt_create_kernelspace_interrupt_entry((uint32_t)&EXTERN_ISR_HANDLER_NAME(x))

#define ISR_MAX_HANDLERS 256

EXTERN_ISR_HANDLER(0);
EXTERN_ISR_HANDLER(1);
EXTERN_ISR_HANDLER(2);
EXTERN_ISR_HANDLER(3);
EXTERN_ISR_HANDLER(4);
EXTERN_ISR_HANDLER(5);
EXTERN_ISR_HANDLER(6);
EXTERN_ISR_HANDLER(7);
EXTERN_ISR_HANDLER(8);
EXTERN_ISR_HANDLER(9);
EXTERN_ISR_HANDLER(10);
EXTERN_ISR_HANDLER(11);
EXTERN_ISR_HANDLER(12);
EXTERN_ISR_HANDLER(13);
EXTERN_ISR_HANDLER(14);
EXTERN_ISR_HANDLER(15);
EXTERN_ISR_HANDLER(16);
EXTERN_ISR_HANDLER(17);
EXTERN_ISR_HANDLER(18);
EXTERN_ISR_HANDLER(19);
EXTERN_ISR_HANDLER(20);
EXTERN_ISR_HANDLER(21);
EXTERN_ISR_HANDLER(22);
EXTERN_ISR_HANDLER(23);
EXTERN_ISR_HANDLER(24);
EXTERN_ISR_HANDLER(25);
EXTERN_ISR_HANDLER(26);
EXTERN_ISR_HANDLER(27);
EXTERN_ISR_HANDLER(28);
EXTERN_ISR_HANDLER(29);
EXTERN_ISR_HANDLER(30);
EXTERN_ISR_HANDLER(31);

typedef struct isr_info {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t interrupt_number, error_code;
    uint32_t eip, cs, eflags, esp_user, ss;
} isr_info_t;

typedef void (* isr_func_ptr)(isr_info_t *context);

void interrupt_handler(isr_info_t *context);
void isr_set_handler(uint32_t interrupt_number, uintptr_t address);

#endif /* ISR_H_ */
