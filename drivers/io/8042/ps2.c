#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <io/8042/ps2.h>
#include <io/port_io.h>
#include <integral/timer.h>

static bool _ps2_is_status(ps2_status_t status, ps2_flags_t flags);

static ps2_drv_state_t _driver_state = { 0 };

void ps2_initialize(void) {
    ps2_set_port_state(PS2_PORT1, false);
    ps2_set_port_state(PS2_PORT2, false);
    ps2_flush_data_port();

    ps2_word_t config = ps2_get_configuration_byte();
    printf("ps2_initialize: initial config byte status is %08b\n", config);

    _driver_state.is_dual_channel = IS_BIT_SET(config, 5);

    CLEAR_BIT(config, 0); // disable irq 1
    CLEAR_BIT(config, 1); // disable irq 12
    CLEAR_BIT(config, 6); // disable translation
    ps2_set_configuration_byte(config);

    ps2_word_t test_response;
    if(!ps2_test_controller(&test_response)) {
        printf("ps2_initialize: PS/2 controller test failed with response 0x%02X\n", test_response);
        return;
    }

    ps2_word_t port_status;
    if(ps2_test_port(PS2_PORT1, &port_status)) {
        _driver_state.ports[PS2_PORT1].operational = true;
    } else {
        printf("ps2_initialize: PS/2 port 1 test failed with response 0x%02X\n", port_status);
    }

    if(_driver_state.is_dual_channel) {
        if(ps2_test_port(PS2_PORT2, &port_status)) {
            _driver_state.ports[PS2_PORT2].operational = true;
        } else {
            printf("ps2_initialize: PS/2 port 2 test failed with response 0x%02X\n", port_status);
        }
    }
    else {
        printf("ps2_initialize: omitting 2nd channel initialization - does not exist.\n");
    }

    if(!_driver_state.ports[PS2_PORT1].operational && !_driver_state.ports[PS2_PORT2].operational) {
        printf("ps2_initialize: all PS/2 ports inoperational, giving up\n");
        return;
    }

    printf("ps2_initialize: post-test config byte status is %08b\n", config);
    if(_driver_state.ports[PS2_PORT1].operational) {
        ps2_set_port_state(PS2_PORT1, true);
        ps2_identify_device(PS2_PORT1);
        SET_BIT(config, 0);
    }

    if(_driver_state.ports[PS2_PORT2].operational) {
        ps2_set_port_state(PS2_PORT2, true);
        ps2_identify_device(PS2_PORT2);
        SET_BIT(config, 1);
    }

    printf("ps2_initialize: final config byte status is %08b\n", config);
    ps2_set_configuration_byte(config);
}

void ps2_send(ps2_port_t port, ps2_word_t packet) {
    uint8_t max_retries = PS2_MAX_RETRIES;

    while(max_retries--) {
        ps2_status_t status = ps2_read_status();

        // must be empty before trying to read, hence input flag = 0
        if(!_ps2_is_status(status, PS2_STATUS_INPUT))
            break;
    }

    if(!max_retries) {
        printf("ps2_send: failed to send command 0x%02X to port 0x%02X after %d retries\n", packet, port, PS2_MAX_RETRIES);
        return;
    }

    outb(port, packet);

    _driver_state.last_port_used = port;
    _driver_state.last_packet_sent = packet;
}

void ps2_send_io(ps2_word_t packet) {
    ps2_send(PS2_PORT_IO, packet);
}

void ps2_resend_last(void) {
    ps2_send(_driver_state.last_port_used, _driver_state.last_packet_sent);
}

bool ps2_read(ps2_word_t* retval) {
    uint8_t max_retries = PS2_MAX_RETRIES;

    while(max_retries--) {
        ps2_status_t status = ps2_read_status();

        // must be full before reading, hence output flag = 1
        if(_ps2_is_status(status, PS2_STATUS_OUTPUT))
            break;
    }

    if(!max_retries) {
        printf("ps2_read: failed to read from PS/2 IO port after %d retries\n", PS2_MAX_RETRIES);
        return false;
    }

    ps2_word_t ret = inb(PS2_PORT_IO);
    *retval = ret;

    return true;
}

ps2_status_t ps2_read_status(void) {
    return inb(PS2_REG_STA);
}

ps2_word_t ps2_get_configuration_byte(void) {
    ps2_send(PS2_REG_CMD, PS2_CMD_GETCONFIGBYTE);

    ps2_word_t retval = 0;
    if(!ps2_read(&retval)) {
        return 0;
    }

    return retval;
}

void ps2_set_configuration_byte(ps2_word_t new_value) {
    asm volatile("cli");

    ps2_send(PS2_REG_CMD, PS2_CMD_SETCONFIGBYTE);
    ps2_send(PS2_PORT_IO, new_value);

    asm volatile("sti");
}

void ps2_set_port_state(uint8_t port, bool enable) {
    char* action = "disable";
    if(enable) action = "enable";

    if(port == PS2_PORT1) {
        if(enable)
            ps2_send(PS2_REG_CMD, PS2_CMD_ENABLEPORT_1);
        else
            ps2_send(PS2_REG_CMD, PS2_CMD_DISABLEPORT_1);
    } else if(port == PS2_PORT2) {
        if(enable)
            ps2_send(PS2_REG_CMD, PS2_CMD_ENABLEPORT_2);
        else
            ps2_send(PS2_REG_CMD, PS2_CMD_DISABLEPORT_2);
    } else printf("ps2_disable_port: tried to %s non-existent port %d\n", action, port);
}

void ps2_flush_data_port(void) {
    ps2_status_t status = ps2_read_status();

    while(_ps2_is_status(status, PS2_STATUS_OUTPUT)) {
        inb(PS2_PORT_IO);
        status = ps2_read_status();
    }
}

ps2_devtype_t ps2_identify_device(uint8_t port) {
    if(port == PS2_PORT2) {
        ps2_send(PS2_REG_CMD, PS2_CMD_SWITCH_PORT2);
    } else if(port != PS2_PORT1 && port != PS2_PORT2) {
        printf("ps2_identify_device: tried to identify a device on non-existent port %d\n", port);
        return -1;
    }

    ps2_data_t data;

    ps2_send(PS2_PORT_IO, PS2_DEVCMD_DISABLE_SCANNING);
    ps2_wait_for_data(&data);
    if(data.response != PS2_DEVRESPONSE_ACK || data.timeout)
        printf("ps2_identify_device: no ACK received or timeout reached: 0x%02X\n", data.response);

    ps2_send(PS2_PORT_IO, PS2_DEVCMD_IDENTIFY);
    ps2_wait_for_data(&data);
    if(data.response != PS2_DEVRESPONSE_ACK || data.timeout)
        printf("ps2_identify_device: no ACK received or timeout reached: 0x%02X\n", data.response);

    bool has_idents = true;
    uint8_t ident_bytes[2] = { 0 };

    ps2_wait_for_data(&data);
    if(!data.timeout) {
       ident_bytes[0] = data.response;
    }
    else {
        has_idents = false;
    }

    if(has_idents) {
        ps2_wait_for_data(&data);
        if(!data.timeout) {
            ident_bytes[1] = data.response;
        }
    }

    switch(ident_bytes[0]) {
        case 0x00:
            _driver_state.ports[port].device = MOUSE_STANDARD;
            break;
        case 0x03:
            _driver_state.ports[port].device = MOUSE_SCROLLWHEEL;
            break;
        case 0x04:
            _driver_state.ports[port].device = MOUSE_FIVEBUTTON;
            break;
        case 0xAB:
            switch(ident_bytes[1]) {
                case 0x41:
                case 0xC1:
                    _driver_state.ports[port].device = KEYBOARD_TRANSLATING;
                    break;
                case 0x83:
                    _driver_state.ports[port].device = KEYBOARD;
                    break;
                default:
                    printf("ps2_identify_device: unexpected device byte 1: 0x%02X", ident_bytes[1]);
                    break;
            }
            break;
        default:
            printf("ps2_identify_device: unexpected device byte 0: 0x%02X", ident_bytes[0]);
            break;
    }

    return _driver_state.ports[port].device;
}

void ps2_wait_for_data(ps2_data_t* out_status) {
    ps2_word_t response = 0;
    bool timeout = false;

    if(!ps2_read(&response)) {
        timeout = true;
    }

    if(out_status) {
        out_status->response = response;
        out_status->timeout = timeout;
    }
}

bool ps2_test_controller(ps2_word_t* out_response) {
    ps2_send(PS2_REG_CMD, PS2_CMD_TESTCNTRLR);
    ps2_word_t response = 0;

    if(!ps2_read(&response))
        return false;

    if(out_response)
        *out_response = response;

    return response == PS2_TESTRESULT_CNTRLR_OK;
}

bool ps2_test_port(uint8_t port, ps2_word_t* out_response) {
    if(port == PS2_PORT1) {
        ps2_send(PS2_REG_CMD, PS2_CMD_TESTPORT_1);
    } else if(port == PS2_PORT2) {
        ps2_send(PS2_REG_CMD, PS2_CMD_TESTPORT_2);
    } else {
        printf("ps2_test_port: tried to test non-existent port %d\n", port);
        return false;
    }

    ps2_word_t response = 0;
    if(!ps2_read(&response))
        return false;

    if(out_response)
        *out_response = response;

    return response == PS2_TESTRESULT_PORT_OK;
}

void ps2_reset_cpu(void) {
    outb(PS2_REG_CMD, PS2_CMD_PULSE_RESETLN);
}

ps2_drv_state_t ps2_get_drv_state(void) {
    return _driver_state;
}

void ps2_wait_io(void) {
    sleep(20);
}

static bool _ps2_is_status(ps2_status_t status, ps2_flags_t flags) {
    return status & flags;
}
