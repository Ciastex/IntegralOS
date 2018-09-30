/*
 * File name: keymap.h
 * Description: 
 *  
 * * * * */

#ifndef INCLUDE_IO_KEYBOARD_KEYMAP_H_
#define INCLUDE_IO_KEYBOARD_KEYMAP_H_

#include <stdint.h>
#include <io/keyboard/keyboard.h>

uint8_t keymap_regular[255] = {
        [VK_ALPHA1] = '1', [VK_ALPHA2] = '2', [VK_ALPHA3] = '3', [VK_ALPHA4] = '4', [VK_ALPHA5] = '5',
        [VK_ALPHA6] = '6', [VK_ALPHA7] = '7', [VK_ALPHA8] = '8', [VK_ALPHA9] = '9', [VK_ALPHA0] = '0',
        [VK_MINUS] = '-', [VK_EQUALS] = '=', [VK_Q] = 'q', [VK_W] = 'w', [VK_E] = 'e', [VK_R] = 'r',
        [VK_T] = 't', [VK_Y] = 'y', [VK_U] = 'u', [VK_I] = 'i', [VK_O] = 'o', [VK_P] = 'p',
        [VK_A] = 'a', [VK_S] = 's', [VK_D] = 'd', [VK_F] = 'f', [VK_G] = 'g', [VK_H] = 'h',
        [VK_J] = 'j', [VK_K] = 'k', [VK_L] = 'l', [VK_Z] = 'z', [VK_X] = 'x', [VK_C] = 'c',
        [VK_V] = 'v', [VK_B] = 'b', [VK_C] = 'c', [VK_N] = 'n', [VK_M] = 'm', [VK_LBRACKET] = '[',
        [VK_RBRACKET] = ']', [VK_SEMICOLON] = ';', [VK_QUOTE] = '\'', [VK_BACKSLASH] = '\\',
        [VK_COMMA] = ',', [VK_PERIOD] = '.', [VK_SLASH] = '/'
};

uint8_t keymap_shifted[255] = {
        [VK_ALPHA1] = '!', [VK_ALPHA2] = '@', [VK_ALPHA3] = '#', [VK_ALPHA4] = '$', [VK_ALPHA5] = '%',
        [VK_ALPHA6] = '^', [VK_ALPHA7] = '&', [VK_ALPHA8] = '*', [VK_ALPHA9] = '(', [VK_ALPHA0] = ')',
        [VK_MINUS] = '_', [VK_EQUALS] = '+', [VK_Q] = 'Q', [VK_W] = 'W', [VK_E] = 'E', [VK_R] = 'R',
        [VK_T] = 'T', [VK_Y] = 'Y', [VK_U] = 'U', [VK_I] = 'I', [VK_O] = 'O', [VK_P] = 'P',
        [VK_A] = 'A', [VK_S] = 'S', [VK_D] = 'D', [VK_F] = 'F', [VK_G] = 'G', [VK_H] = 'H',
        [VK_J] = 'J', [VK_K] = 'K', [VK_L] = 'L', [VK_Z] = 'Z', [VK_X] = 'X', [VK_C] = 'C',
        [VK_V] = 'V', [VK_B] = 'B', [VK_C] = 'C', [VK_N] = 'N', [VK_M] = 'M', [VK_LBRACKET] = '{',
        [VK_RBRACKET] = '}', [VK_SEMICOLON] = ':', [VK_QUOTE] = '"', [VK_BACKSLASH] = '|',
        [VK_COMMA] = '<', [VK_PERIOD] = '>', [VK_SLASH] = '?'
};

#endif /* INCLUDE_IO_KEYBOARD_KEYMAP_H_ */
