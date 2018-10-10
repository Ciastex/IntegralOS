#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <io/keyboard/keyboard.h>
#include <io/keyboard/keymap.h>
#include <io/8042/ps2.h>
#include <hal/irq.h>

static bool _kbd_send_io(ps2_word_t packet, ps2_data_t* data);
static void _kbd_internal(irq_info_t* irq);

static uint16_t _current_handler_id = 0;
static kbd_keypress_handler_t* _keypress_handlers[KBD_MAX_KEYPRESS_HANDLERS] = { 0 };

static kbd_status_t _status = { 0 };

void kbd_initialize(void) {
    if(!kbd_reset()) {
        printf("kbd_initialize: reset failed.\n");
    }

    kbd_set_scancode_set(SCANCODE_SET_3);
    kbd_set_scanning(true);

    irq_set_handler(1, &_kbd_internal);
}

uint16_t kbd_add_keypress_handler(kbd_keypress_handler_t* handler) {
    if(_current_handler_id + 1 > KBD_MAX_KEYPRESS_HANDLERS) return KBD_KEYPRESS_HANDLER_LIMIT_EXCEEDED;
    uint16_t ret = _current_handler_id;

    _keypress_handlers[_current_handler_id++] = handler;

    return ret;
}

bool kbd_remove_keypress_handler(uint16_t id) {
    if(!_current_handler_id) return false;
    if(!_keypress_handlers[id]) return false;

    _keypress_handlers[id] = 0;

    for(uint16_t i = id; i < KBD_MAX_KEYPRESS_HANDLERS; i++) {
        if(id + 1 > KBD_MAX_KEYPRESS_HANDLERS) break;
        if(!_keypress_handlers[i + 1]) break;

        kbd_keypress_handler_t* tmp = _keypress_handlers[i + 1];
        _keypress_handlers[i + 1] = 0;
        _keypress_handlers[i] = tmp;
    }

    _current_handler_id--;
    return true;
}

bool kbd_reset(void) {
    ps2_data_t data;

    if(!_kbd_send_io(KBDCMD_INIT, &data)) {
        if(data.timeout) {
            printf("kbd_reset: timeout while sending command. giving up.\n");
        } else printf("kbd_reset: unexpected response while sending command. giving up.\n");

        return false;
    }

    ps2_wait_for_data(&data);
    if(data.timeout) {
        printf("kbd_reset: timeout while getting test result. giving up.\n");
        return false;
    }

    while(data.response == PS2_DEVRESPONSE_ACK) {
        ps2_wait_for_data(&data);
    }

    if(data.response == KBDRESPONSE_TESTFAILED_1 || data.response == KBDRESPONSE_TESTFAILED_2) {
        printf("kbd_reset: init test returned failure.\n");
        return false;
    } else if(data.response != KBDRESPONSE_INIT_OK) {
        printf("kbd_reset: init test returned unexpected value: 0x%02X\n", data.response);
        return false;
    }

    return true;
}

kbd_scancode_set_t kbd_get_current_scancode_set(void) {
    ps2_data_t data;

    if(!_kbd_send_io(KBDCMD_SCANCODES, &data)) {
        if(data.timeout) {
            printf("kbd_get_current_scancode_set: timeout\n");
        } else printf("kbd_get_current_scancode_set: invalid response 0x%02X\n", data.response);

        return SCANCODE_ERROR;
    }

    if(!_kbd_send_io(KBDSCANCODE_GET, &data)) {
        if(data.timeout) {
            printf("kbd_get_current_scancode_set: no scancode received\n");
        } else printf("kbd_get_current_scancode_set: invalid response 0x%02X\n", data.response);

        return SCANCODE_ERROR;
    }

    ps2_wait_for_data(&data);
    if(data.timeout) {
        return SCANCODE_ERROR;
    }

    return (kbd_scancode_set_t)data.response;
}

void kbd_set_scancode_set(kbd_scancode_set_t set) {
    ps2_data_t data;

    if(!_kbd_send_io(KBDCMD_SCANCODES, &data)) {
        if(data.timeout) {
            printf("kbd_set_scancode_set: timeout\n");
        } else printf("kbd_scancode_set: invalid response 0x%02X\n", data.response);

        return;
    }

    if(!_kbd_send_io(set, &data)) {
        if(data.timeout) {
            printf("kbd_set_scancode_set: can't be sure if scancode is set; timeout\n");
        } else printf("kbd_set_scancode_set: invalid response 0x%02X\n", data.response);

        while(ps2_can_read()) {
            ps2_wait_for_data(&data);
            printf("%02X\n", data.response);
        }
    }
}

void kbd_set_scanning(bool scanning) {
    ps2_data_t data;
    ps2_word_t packet = scanning ? PS2_DEVCMD_ENABLE_SCANNING : PS2_DEVCMD_DISABLE_SCANNING;

    if(!_kbd_send_io(packet, &data)) {
        if(data.timeout) {
            printf("kbd_set_scanning: timeout");
        } else printf("kbd_set_scanning: invalid response 0x%02X\n", data.response);
    }
}

static bool _kbd_send_io(ps2_word_t packet, ps2_data_t* data) {
    bool ret = false;
    uint8_t retries = PS2_MAX_RETRIES;

    ps2_send_io(packet);
    ps2_wait_for_data(data);

    if(data->timeout)
        return false;

    if(data->response == PS2_DEVRESPONSE_RESEND) {
        printf("_kbd_send_io: resending 0x%02X\n", packet);

        while(retries--) {
            ps2_resend_last();
            ps2_wait_for_data(data);

            if(data->timeout)
                return false;

            if(data->response != PS2_DEVRESPONSE_RESEND)
                break;
        }

        if(!retries)
            return false;

        if(data->response == PS2_DEVRESPONSE_ACK) {
            ret = true;
        }
    } else if(data->response == PS2_DEVRESPONSE_ACK) {
        ret = true;
    }

    return ret;
}

static void _kbd_internal(irq_info_t* irq) {
    if(ps2_can_read()) {
        bool pressed = true;

        ps2_data_t data;
        ps2_wait_for_data(&data);

        if(data.response == 0xF0) {
            pressed = false;
            ps2_wait_for_data(&data);
        }

        // laptop keyboard?
        //if(data.response == 0xE0) {
        //    pressed = true;
        //    ps2_wait_for_data(&data);
        //}

        switch(data.response) {
            case VK_LSHIFT: _status.control_keys.lshift = pressed; break;
            case VK_RSHIFT: _status.control_keys.rshift = pressed; break;
            case VK_LALT:   _status.control_keys.lalt = pressed; break;
            case VK_RALT:   _status.control_keys.ralt = pressed; break;
            case VK_LCTRL:  _status.control_keys.lctrl = pressed; break;
            case VK_RCTRL:  _status.control_keys.rctrl = pressed; break;
            case VK_LMETA:  _status.control_keys.lmeta = pressed; break;
            case VK_RMETA:  _status.control_keys.rmeta = pressed; break;
            default: break;
        }

        char character = keymap_unshifted[data.response];
        if(_status.control_keys.lshift || _status.control_keys.rshift) {
            character = keymap_shifted[data.response];
        }

        kbd_event_data_t evdata;
        evdata.scancode = data.response;
        evdata.control_keys = _status.control_keys;
        evdata.pressed = pressed;
        evdata.character = character;

        for(uint16_t i = 0; i < KBD_MAX_KEYPRESS_HANDLERS; i++) {
            if(!_keypress_handlers[i]) break;
            _keypress_handlers[i](evdata);
        }
    }
    ps2_flush_data_port();
}
