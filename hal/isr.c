/*
 * File name: isr.c
 * Description: 
 *
 * * * */
#include <stdint.h>
#include <stdio.h>

#include <hal/isr.h>
#include <integral/panic.h>

void interrupt_handler(isr_info_t *context) {
    const char *msg = "Unhandled exception occured.";

    if(context->interrupt_number < 32) {
        switch(context->interrupt_number) {
            case 0:
                msg = "Division by zero exception [#DE]";
                break;
            case 1:
                msg = "Debug trap [#DB]";
                break;
            case 2:
                msg = "Non-maskable interrupt";
                break;
            case 3:
                msg = "Breakpoint trap [#BP]";
                break;
            case 4:
                msg = "Integer overflow [#OF]";
                break;
            case 5:
                msg = "Bound range exceeded [#BR]";
                break;
            case 6:
                msg = "Invalid opcode [#UD]";
                break;
            case 7:
                msg = "Device not available [#NM]";
                break;
            case 8:
                msg = "Double fault [#DF]";
                break;
            case 9:
                msg = "Coprocessor semgent overrun";
                break;
            case 10:
                msg = "Invalid TSS [#TS]";
                break;
            case 11:
                msg = "Segment is not present [#NP]";
                break;
            case 12:
                msg = "Stack segment fault [#SS]";
                break;
            case 13:
                msg = "General protection fault [#GP]";
                break;
            case 14:
                msg = "Page fault [#PF]";
                break;
            case 16:
                msg = "x87 floating point exception [#MF]";
                break;
            case 17:
                msg = "Alignment check [#AC]";
                break;
            case 18:
                msg = "Machine check [#MC]";
                break;
            case 19:
                msg = "SIMD floating point exception [#XF]";
                break;
            case 20:
                msg = "Virtualization exception [#VE]";
                break;
            case 30:
                msg = "Security exception [#SX]";
                break;
            case 15:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 31:
                msg = "Reserved exception";
                break;
        }
        kpanic(msg, context);
    }
}

