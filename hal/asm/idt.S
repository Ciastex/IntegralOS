#
# File name: idt.S
# Description: Low-level IDT replacement routine
#
# # # # #

.global low_level_load_idt

low_level_load_idt:
	mov 4(%esp), %eax
	lidt (%eax)
	ret
