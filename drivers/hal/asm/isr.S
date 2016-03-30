#
# File name: isr.S
# Description: Low level interrupt service routine preambles.
#
# # # # #

.macro ISR_NOCODE number
	.global isr_handler_\number
	isr_handler_\number:
		cli
		pushl $0
		pushl $\number
		jmp isr_common_handler
.endm

.macro ISR_CODE number
	.global isr_handler_\number
	isr_handler_\number:
		cli
		pushl $\number
		jmp isr_common_handler
.endm

ISR_NOCODE 0
ISR_NOCODE 1
ISR_NOCODE 2
ISR_NOCODE 3
ISR_NOCODE 4
ISR_NOCODE 5
ISR_NOCODE 6
ISR_NOCODE 7
ISR_CODE 8
ISR_NOCODE 9
ISR_CODE 10
ISR_CODE 11
ISR_CODE 12
ISR_CODE 13
ISR_CODE 14
ISR_NOCODE 15
ISR_NOCODE 16
ISR_CODE 17
ISR_NOCODE 18
ISR_NOCODE 19
ISR_NOCODE 20
ISR_NOCODE 21
ISR_NOCODE 22
ISR_NOCODE 23
ISR_NOCODE 24
ISR_NOCODE 25
ISR_NOCODE 26
ISR_NOCODE 27
ISR_NOCODE 28
ISR_NOCODE 29
ISR_CODE 30
ISR_NOCODE 31

.extern interrupt_handler
isr_common_handler:
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

	call interrupt_handler
	popl %eax
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa

	add $8, %esp
	iret
