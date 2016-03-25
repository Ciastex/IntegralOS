#
# File name: boot.S
# Description: Basic multi-boot header set-up and jump to a higher-level C language.
#
# # # # #

.set ALIGN,		1 << 0
.set MEMINFO,	1 << 1
.set FLAGS, 	ALIGN | MEMINFO
.set MAGIC,		0x1BADB002
.set CHECKSUM,	-(MAGIC + FLAGS)

.section .multiboot_header
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Boot-time stack setup.
.section .boot_stack, "aw", @nobits
stack_bottom:
	.skip 16384
stack_top:
	# There's nothing to see here, move along.

.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp
	call kernel_init

	cli
	hlt

.failsafe_loop:
	jmp .failsafe_loop

.size _start, . - _start
