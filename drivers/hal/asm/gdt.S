.intel_syntax noprefix
.global gdt_load_ll

gdt_load_ll:
    mov eax, [esp + 4]
    lgdt [eax]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:finish

    finish:
        ret

# at&t syntax left for reference
#
#low_level_load_gdt:
#    movl 4(%esp), %eax
#    lgdt (%eax)
#    mov $0x10, %ax
#    mov %ax, %ds
#    mov %ax, %es
#    mov %ax, %fs
#    mov %ax, %gs
#    mov %ax, %ss
#
#    ljmp $0x08,$done
#done:
#    ret
