/*
 * File name: panic.c
 * Description: Kernel panic implementation.
 *
 * * * */

#include <stdio.h>
#include <hal/isr.h>

void kpanic(const char *message) {
    printf("[\\[4INTEGRAL OS KERNEL PANIC\\X]: %s", message);

    /*if(context) {
        printf("\n\nCRITICAL CPU EXCEPTION: 0x%02X\n", context->interrupt_number);
        printf("Register contents:\n    \\[9EAX\\X 0x%08X  \\[9ECX\\X 0x%08X    \\[ECS\\X 0x%04X  \\[EDS\\X 0x%04X  \\[EES\\X 0x%04X\n", context->eax, context->ecx, context->cs, context->ds, context->es);
        printf("    \\[9EBX\\X 0x%08X  \\[9EDX\\X 0x%08X    \\[EFS\\X 0x%04X  \\[EGS\\X 0x%04X  \\[ESS\\X 0x%08X\n", context->ebx, context->edx, context->fs, context->gs, context->ss);
        printf("    \\[9ESP\\X 0x%08X  \\[9EIP\\X 0x%08X\n", context->esp, context->eip);
    }*/

    asm volatile("cli");
    asm volatile("hlt");
}
