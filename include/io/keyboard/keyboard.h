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

#define KBDSCANCODE_GET 0x00

#define KBD_MAX_KEYPRESS_HANDLERS 0xFF
#define KBD_KEYPRESS_HANDLER_LIMIT_EXCEEDED 0xFFFE

typedef uint8_t kbd_scancode_t;

typedef enum kbd_scancode_set {
    SCANCODE_SET_1 = 0x01,
    SCANCODE_SET_2 = 0x02,
    SCANCODE_SET_3 = 0x03,
    SCANCODE_ERROR = 0xFF
} kbd_scancode_set_t;

typedef struct kbd_control_keys {
    bool caps_lock;
    bool lctrl;
    bool rctrl;
    bool lalt;
    bool ralt;
    bool lshift;
    bool rshift;
    bool lmeta;
    bool rmeta;
} kbd_control_keys_t;

typedef struct kbd_status {
    kbd_scancode_t     last_scancode;
    kbd_control_keys_t control_keys;
} kbd_status_t;

typedef struct kbd_event_data {
    kbd_scancode_t scancode;
    kbd_control_keys_t control_keys;
    bool pressed;
    char character;
} kbd_event_data_t;

typedef void kbd_keypress_handler_t(kbd_event_data_t data);

void kbd_initialize(void);
bool kbd_reset(void);
kbd_scancode_set_t kbd_get_current_scancode_set(void);
void kbd_set_scancode_set(kbd_scancode_set_t set);
void kbd_set_scanning(bool scanning);
uint16_t kbd_add_keypress_handler(kbd_keypress_handler_t* handler);
bool kbd_remove_keypress_handler(uint16_t id);
#endif // INCLUDE_IO_KEYBOARD_KEYBOARD_H_
