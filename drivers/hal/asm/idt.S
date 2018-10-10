.intel_syntax noprefix
.global idt_load_ll

idt_load_ll:
    cli
    mov eax, [esp + 4]
    lidt [eax]
    ret

#idt_load_ll:
#   cli
#   mov 4(%esp), %eax
#   lidt (%eax)
#   ret
