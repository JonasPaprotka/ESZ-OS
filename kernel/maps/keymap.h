// File Updated with AI / Assisted by AI

#ifndef KEYMAPS_H
#define KEYMAPS_H

#include <stdint.h>

enum KeyboardLayout {
    LAYOUT_US,
    LAYOUT_DE
};

enum KeyCode : uint16_t {
    KEY_UNKNOWN = 0,
    KEY_ENTER = 256,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_LALT,
    KEY_RALT,
    KEY_CAPSLOCK,
    KEY_ESCAPE,
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

    KEY_DE_AE = 500, KEY_DE_OE, KEY_DE_UE, KEY_DE_SS,
    KEY_DE_AE_CAPS, KEY_DE_OE_CAPS, KEY_DE_UE_CAPS
};

extern uint16_t us_keymap[2][0x3A];
extern uint16_t de_keymap[2][0x3A];

uint16_t scancode_to_keycode(const uint8_t scancode);

#endif // KEYMAPS_H
