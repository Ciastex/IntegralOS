#ifndef INCLUDE_IO_KEYBOARD_KEYBOARD_H_
#define INCLUDE_IO_KEYBOARD_KEYBOARD_H_

#include <stdint.h>
#include <stdbool.h>

#define KBDCMD_SETLEDS   0xED
#define KBDCMD_ECHO      0xEE
#define KBDCMD_SCANCODES 0xF0
#define KBDCMD_TYPEMATIC 0xF3
#define KBDCMD_INIT      0xFF

#define KBDRESPONSE_BUF_ERR_1    0x00
#define KBDRESPONSE_INIT_OK      0xAA
#define KBDRESPONSE_ECHO         0xEE
#define KBDRESPONSE_TESTFAILED_1 0xFC
#define KBDRESPONSE_TESTFAILED_2 0xFD
#define KBDRESPONSE_BUF_ERR_2    0xFF

void kbd_initialize(void);
bool kbd_reset(void);

#endif // INCLUDE_IO_KEYBOARD_KEYBOARD_H_
