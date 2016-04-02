/*
 * File name: panic.c
 * Description: Kernel panic implementation.
 *
 * * * */

#include <stdio.h>
#include <stdarg.h>
#include <display/vga.h>

void kpanic(const char *message, uint32_t ex_data_0, uint32_t ex_data_1, uint32_t ex_data_2) {
	uint32_t eax, ebx, ecx, edx;
	uint32_t esi, edi;
	uint32_t cr0, cr2, cr3, cr4;
	uint16_t cs, ds, es, fs, gs, ss;

	asm volatile("movl %%eax, %0" : "=r"(eax));
	asm volatile("movl %%ebx, %0" : "=r"(ebx));
	asm volatile("movl %%ecx, %0" : "=r"(ecx));
	asm volatile("movl %%edx, %0" : "=r"(edx));

	asm volatile("movl %%esi, %0" : "=r"(esi));
	asm volatile("movl %%edi, %0" : "=r"(edi));

	asm volatile("movl %%cr0, %0" : "=r"(cr0));
	asm volatile("movl %%cr2, %0" : "=r"(cr2));
	asm volatile("movl %%cr3, %0" : "=r"(cr3));
	asm volatile("movl %%cr4, %0" : "=r"(cr4));

	asm volatile("movw %%cs, %0" : "=r"(cs));
	asm volatile("movw %%ds, %0" : "=r"(ds));
	asm volatile("movw %%es, %0" : "=r"(es));
	asm volatile("movw %%fs, %0" : "=r"(fs));
	asm volatile("movw %%gs, %0" : "=r"(gs));
	asm volatile("movw %%ss, %0" : "=r"(ss));

    printf("\n\n[\\[4INTEGRAL OS KERNEL PANIC\\X]: %s\n", message);

    printf("  Registers dump:\n");
    printf("    [\\[9EAX\\X]: 0x%08X  [\\[9EBX\\X]: 0x%08X  [\\[9ECX\\X]: 0x%08X  [\\[9EDX\\X]: 0x%08X\n", eax, ebx, ecx, edx);
    printf("    [\\[9ESI\\X]: 0x%08X  [\\[9EDI\\X]: 0x%08X\n\n", esi, edi);
    printf("    [\\[ACR0\\X]: 0x%08X  [\\[ACR2\\X]: 0x%08X  [\\[ACR3\\X]: 0x%08X  [\\[ACR4\\X]: 0x%08X\n", cr0, cr2, cr3, cr4);
    printf("    [\\[ECS\\X]: 0x%04X  [\\[EDS\\X]: 0x%04X  [\\[EES\\X]: 0x%04X\n", cs, ds, es);
    printf("    [\\[EFS\\X]: 0x%04X  [\\[EGS\\X]: 0x%04X  [\\[ESS\\X]: 0x%04X\n\n", fs, gs, ss);

    printf("  \\[3Addtitional exception data\\X:\n");
    printf("    [\\[70\\X]: 0x%08X %032b %d\n", ex_data_0, ex_data_0, ex_data_0);
    printf("    [\\[71\\X]: 0x%08X %032b %d\n", ex_data_1, ex_data_1, ex_data_1);
    printf("    [\\[72\\X]: 0x%08X %032b %d\n", ex_data_2, ex_data_2, ex_data_2);
    vga_disable_cursor();

    asm volatile("cli");
    asm volatile("hlt");
}
