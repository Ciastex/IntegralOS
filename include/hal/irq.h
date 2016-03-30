/*
 * File name: irq.h
 * Description: x86 IRQ header interface
 *
 * * * */

#ifndef IRQ_H_
#define IRQ_H_

#include <stdint.h>

#define EXTERN_IRQ_HANDLER_NAME(x) irq_handler_ ## x
#define EXTERN_IRQ_HANDLER(x) extern void EXTERN_IRQ_HANDLER_NAME(x)(void)
#define IRQ_INT_HANDLER(x) idt_entries[x] = create_kernelspace_interrupt_entry((uint32_t)&EXTERN_IRQ_HANDLER_NAME(x))

#define IRQ_NUMBER_PIT  0x00
#define IRQ_NUMBER_KBD  0x01
#define IRQ_NUMBER_SLV  0x02 // PIC cascade for slave
#define IRQ_NUMBER_COM2 0x03
#define IRQ_NUMBER_COM3 0x04
#define IRQ_NUMBER_RSD1 0x05 // Reserved #1
#define IRQ_NUMBER_FDC  0x06
#define IRQ_NUMBER_LPT1 0x07
#define IRQ_NUMBER_CMOS 0x08
#define IRQ_NUMBER_CGA  0x09
#define IRQ_NUMBER_RSD2 0x0A
#define IRQ_NUMBER_RSD3 0x0B
#define IRQ_NUMBER_RSD4 0x0C
#define IRQ_NUMBER_FPU  0x0D
#define IRQ_NUMBER_HDC  0x0E
#define IRQ_NUMBER_RSD5 0x0F

typedef struct irq_info {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t irq_number, irq_code;
    uint32_t eip, cs, eflags, esp_user, ss;
} irq_info_t;

typedef void (* irq_func_ptr)(irq_info_t *irq_info);

EXTERN_IRQ_HANDLER(32);
EXTERN_IRQ_HANDLER(33);
EXTERN_IRQ_HANDLER(34);
EXTERN_IRQ_HANDLER(35);
EXTERN_IRQ_HANDLER(36);
EXTERN_IRQ_HANDLER(37);
EXTERN_IRQ_HANDLER(38);
EXTERN_IRQ_HANDLER(39);
EXTERN_IRQ_HANDLER(40);
EXTERN_IRQ_HANDLER(41);
EXTERN_IRQ_HANDLER(42);
EXTERN_IRQ_HANDLER(43);
EXTERN_IRQ_HANDLER(44);
EXTERN_IRQ_HANDLER(45);
EXTERN_IRQ_HANDLER(46);

void irq_handler(irq_info_t *irq);
void set_irq_handler(uint32_t number, uint32_t func_pointer);

#endif /* IRQ_H_ */
