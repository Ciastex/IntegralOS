#
# File name: gdt.S
# Description:
#
# # # # #

.global low_level_load_gdt

low_level_load_gdt:
	movl 4(%esp), %eax
	lgdt (%eax)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	ljmp $0x08,$done
done:
	ret
