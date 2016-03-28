/*
 * File name: isr.h
 * Description: 
 *
 * * * */

#ifndef ISR_H_
#define ISR_H_

#include <stdint.h>

extern void isr_handler_0(void);
extern void isr_handler_1(void);
extern void isr_handler_2(void);
extern void isr_handler_3(void);
extern void isr_handler_4(void);
extern void isr_handler_5(void);
extern void isr_handler_6(void);
extern void isr_handler_7(void);
extern void isr_handler_8(void);
extern void isr_handler_9(void);
extern void isr_handler_10(void);
extern void isr_handler_11(void);
extern void isr_handler_12(void);
extern void isr_handler_13(void);
extern void isr_handler_14(void);
extern void isr_handler_15(void);
extern void isr_handler_16(void);
extern void isr_handler_17(void);
extern void isr_handler_18(void);
extern void isr_handler_19(void);
extern void isr_handler_20(void);
extern void isr_handler_21(void);
extern void isr_handler_22(void);
extern void isr_handler_23(void);
extern void isr_handler_24(void);
extern void isr_handler_25(void);
extern void isr_handler_26(void);
extern void isr_handler_27(void);
extern void isr_handler_28(void);
extern void isr_handler_29(void);
extern void isr_handler_30(void);
extern void isr_handler_31(void);

typedef struct isr_info {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t interrupt_number, error_code;
    uint32_t eip, cs, eflags, esp_user, ss;
} __attribute((packed))isr_info_t;

void interrupt_handler(isr_info_t *context);

#endif /* ISR_H_ */
