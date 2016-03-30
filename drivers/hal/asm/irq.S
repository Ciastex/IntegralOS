#
# File name: irq.S
# Description: Low-level IRQ entry handlers
#
# # # # #

.macro IRQ_ENTRY irq h_no
	.global irq_handler_\h_no
	irq_handler_\h_no:
		cli
		pushl $0
		pushl $\irq
		jmp irq_common_handler
.endm

IRQ_ENTRY 0, 32
IRQ_ENTRY 1, 33
IRQ_ENTRY 2, 34
IRQ_ENTRY 3, 35
IRQ_ENTRY 4, 36
IRQ_ENTRY 5, 37
IRQ_ENTRY 6, 38
IRQ_ENTRY 7, 39
IRQ_ENTRY 8, 40
IRQ_ENTRY 9, 41
IRQ_ENTRY 10, 42
IRQ_ENTRY 11, 43
IRQ_ENTRY 12, 44
IRQ_ENTRY 13, 45
IRQ_ENTRY 14, 46
IRQ_ENTRY 15, 47

.extern irq_handler
irq_common_handler:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax

	call irq_handler

	popl %eax
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	add $8, %esp
	iret
