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

typedef struct irq_info {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t irq_number, irq_code;
    uint32_t eip, cs, eflags, esp_user, ss;
} irq_info_t;

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

#endif /* IRQ_H_ */
