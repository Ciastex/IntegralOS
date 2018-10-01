/*
 * File name: keyboard.c
 * Description: Keyboard driver implementation.
 *  
 * * * * */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io/port_io.h>
#include <hal/irq.h>
#include <io/keyboard/keyboard.h>
#include <io/keyboard/keymap.h>

//
// 0 = shift_pressed
// 1 = ctrl_pressed
// 2 = meta_pressed
// 3 = alt_pressed
// 4 = capslock_active
static uint8_t control_flags = 0;

static void kbd_internal_callback(irq_info_t *irq_info);
static uint8_t kbd_translate_scancode_to_keycode(uint8_t scancode);

static keyboard_event_handler_t key_pressed = NULL;
static keyboard_event_handler_t key_released = NULL;

void kbd_init(void) {
    irq_set_handler(1, (uintptr_t)&kbd_internal_callback);
}

bool kbd_is_shift_pressed(void) {
    return IS_BIT_SET(control_flags, 0);
}

bool kbd_is_ctrl_pressed(void) {
    return IS_BIT_SET(control_flags, 1);
}

bool kbd_is_meta_pressed(void) {
    return IS_BIT_SET(control_flags, 2);
}

bool kbd_is_alt_pressed(void) {
    return IS_BIT_SET(control_flags, 3);
}

bool kbd_is_capslock_active(void) {
    return IS_BIT_SET(control_flags, 4);
}

uint8_t kbd_read_status_register(void) {
    return inb(KBD_CONTROL_STATUS_REG);
}

uint8_t kbd_read_encoder_buffer(void) {
    return inb(KBD_ENCODER_INPUT_BUFFER);
}

void kbd_set_pressed_callback(keyboard_event_handler_t handler) {
    key_pressed = handler;
}

void kbd_set_released_callback(keyboard_event_handler_t handler) {
    key_released = handler;
}

void kbd_send_control_command(uint8_t packet) {
    while(1) {
        if(!(kbd_read_status_register() & KBD_STATUS_INPUT)) {
            break;
        }
    }
    outb(KBD_CONTROL_COMMAND_REG, packet);
}

void kbd_send_encoder_command(uint8_t packet) {
    while(1) {
        if(!(kbd_read_status_register() & KBD_STATUS_INPUT)) {
            break;
        }
    }
    outb(KBD_ENCODER_COMMAND_PORT, packet);
}

void kbd_set_leds(bool caps_lock, bool num_lock, bool scroll_lock) {
    uint8_t packet = 0;

    if(caps_lock) {
        packet |= KBD_LED_CAPS_LOCK;
    } else {
        CLEAR_BIT(packet, 2);
    }

    if(num_lock) {
        packet |= KBD_LED_NUM_LOCK;
    } else {
        CLEAR_BIT(packet, 1);
    }

    if(scroll_lock) {
        packet |= KBD_LED_SCROLL_LOCK;
    } else {
        CLEAR_BIT(packet, 0);
    }

    kbd_send_encoder_command(0xED);
    kbd_send_encoder_command(packet);
}

static void kbd_internal_callback(irq_info_t *irq_info) {
    if(inb(KBD_ENCODER_INPUT_BUFFER)) {
        uint8_t scancode = inb(KBD_ENCODER_INPUT_BUFFER);
        uint8_t keycode = kbd_translate_scancode_to_keycode(scancode);

        uint8_t character = keymap_regular[keycode];

        if(IS_BIT_SET(control_flags, 0)) {
            character = keymap_shifted[keycode];
        }

        key_info_t key_info;
        key_info.scan_code = scancode;
        key_info.key_code = keycode;
        key_info.character = character;
        key_info.pressed = false;

        if(scancode & KBD_KEYSTATE_PRESSED) {
            switch(keycode) {
                case VK_LSHIFT:
                case VK_RSHIFT:
                    CLEAR_BIT(control_flags, 0);
                    break;
                case VK_CONTROL:
                    CLEAR_BIT(control_flags, 1);
                    break;
                case VK_META:
                    CLEAR_BIT(control_flags, 2);
                    break;
                case VK_ALT:
                    CLEAR_BIT(control_flags, 3);
                    break;
                default: break;
            }

            if(key_released) key_released(key_info);
        } else {
            switch(keycode) {
                case VK_LSHIFT:
                case VK_RSHIFT:
                    SET_BIT(control_flags, 0);
                    break;
                case VK_CONTROL:
                    SET_BIT(control_flags, 1);
                    break;
                case VK_META:
                    SET_BIT(control_flags, 2);
                    break;
                case VK_ALT:
                    SET_BIT(control_flags, 3);
                    break;
                case VK_CAPSLOCK:
                    TOGGLE_BIT(control_flags, 4);
                    break;
                default: break;
            }

            key_info.pressed = true;
            if(key_pressed) key_pressed(key_info);
        }
    }
}

static uint8_t kbd_translate_scancode_to_keycode(uint8_t scancode) {
    return scancode & ~(1 << 7);
}
