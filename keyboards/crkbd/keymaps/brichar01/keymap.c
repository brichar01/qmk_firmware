/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include "action_util.h"
#include "community_modules.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "progmem.h"
#include "quantum.h"
#include "quantum_keycodes.h"
#include "util.h"
#include QMK_KEYBOARD_H

#include "action.h"
#include "process_combo.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[0] = LAYOUT_split_3x6_3(
                                                                XXXXXXX, XXXXXXX, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, XXXXXXX, XXXXXXX,
                                                                KC_Q, KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_QUOTE,
                                                                XXXXXXX, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, XXXXXXX,
                                                                XXXXXXX, QK_TRI_LAYER_LOWER, QK_USER_0, KC_SPC, QK_TRI_LAYER_UPPER, XXXXXXX),
                                                              [1] = LAYOUT_split_3x6_3(
                                                                _______, _______, QK_MOUSE_BUTTON_4, C(KC_F), QK_MOUSE_BUTTON_5, KC_PAGE_UP, KC_PAGE_UP, KC_HOME, KC_END, _______, _______, _______,
                                                                KC_ESC, A(KC_TAB), C(KC_X), C(KC_C), C(KC_V), QK_REP, KC_ENT, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_PRINT_SCREEN,
                                                                _______, C(KC_Z), _______, _______, _______, KC_LGUI, KC_PAGE_DOWN, KC_BSPC, KC_DEL, KC_TAB, QK_MOUSE_BUTTON_2, _______,
                                                                _______, _______, MO(4), MO(4), _______, _______),
                                                              [2] = LAYOUT_split_3x6_3(
                                                                _______, _______, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_EQL, KC_GRAVE, KC_SEMICOLON, KC_COLON, _______, _______,
                                                                KC_EXLM, _______, _______, _______, _______, KC_CIRC, KC_AMPERSAND, KC_ASTR, KC_EQL, KC_UNDERSCORE, KC_MINUS, _______,
                                                                _______, _______, _______, KC_BACKSLASH, KC_PIPE, KC_PLUS, KC_TILDE, _______, _______, _______, _______, _______,
                                                                _______,  _______, _______, _______, _______, _______),
                                                              [3] = LAYOUT_split_3x6_3(
                                                                _______, _______, _______, _______, _______, _______, KC_EQL, KC_7, KC_8, KC_9, _______, _______,
                                                                _______, _______, _______, _______, _______, _______, KC_ASTR, KC_4, KC_5, KC_6, KC_MINUS, KC_PLUS,
                                                                _______, _______, _______, _______, _______, _______, KC_0, KC_1, KC_2, KC_3, KC_SLSH, _______,
                                                                _______, _______, _______, _______, _______, _______),
                                                              [4] = LAYOUT_split_3x6_3(
                                                                _______, _______, _______, _______, _______, _______, KC_F12, KC_F7, KC_F8, KC_F9, _______, _______,
                                                                _______, _______, _______, _______, _______, _______, KC_F11, KC_F4, KC_F5, KC_F6, _______, _______,
                                                                _______, _______, _______, _______, _______, _______, KC_F10, KC_F1, KC_F2, KC_F3, _______, _______,
                                                                _______, _______, _______, _______, _______, _______),
};
// clang-format on

typedef enum {
    CMB_LRPN,
    CMB_RPRN,
    CMB_LBRC,
    CMB_RBRC,
    CMB_LCBRC,
    CMB_RCBRC,
    CMB_CTRL,
    CMB_SHFT,
    CMB_ALT,
    CMB_SUPR,
} combos_e;

const uint16_t PROGMEM cmb_lprn[]  = {KC_T, KC_V, COMBO_END};
const uint16_t PROGMEM cmb_rprn[]  = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM cmb_lbrc[]  = {KC_S, KC_C, COMBO_END};
const uint16_t PROGMEM cmb_rbrc[]  = {KC_E, KC_COMMA, COMBO_END};
const uint16_t PROGMEM cmb_lcbrc[] = {KC_R, KC_X, COMBO_END};
const uint16_t PROGMEM cmb_rcbrc[] = {KC_I, KC_DOT, COMBO_END};
const uint16_t PROGMEM cmb_ctrl[]  = {KC_N, KC_T, COMBO_END};
const uint16_t PROGMEM cmb_shft[]  = {KC_S, KC_E, COMBO_END};
const uint16_t PROGMEM cmb_alt[]   = {KC_R, KC_I, COMBO_END};
const uint16_t PROGMEM cmb_supr[]  = {KC_A, KC_O, COMBO_END};

// clang-format off
combo_t key_combos[] = {
    [CMB_LRPN] = COMBO(cmb_lprn, KC_LPRN),
    [CMB_RPRN] = COMBO(cmb_rprn, KC_RPRN),
    [CMB_LBRC] = COMBO(cmb_lbrc, KC_LEFT_BRACKET),
    [CMB_RBRC] = COMBO(cmb_rbrc, KC_RIGHT_BRACKET),
    [CMB_LCBRC] = COMBO(cmb_lcbrc, KC_LEFT_CURLY_BRACE),
    [CMB_RCBRC] = COMBO(cmb_rcbrc, KC_RIGHT_CURLY_BRACE),
    [CMB_CTRL] = COMBO_ACTION(cmb_ctrl),
    [CMB_SHFT] = COMBO_ACTION(cmb_shft),
    [CMB_ALT] = COMBO_ACTION(cmb_alt),
    [CMB_SUPR] = COMBO_ACTION(cmb_supr),
};
// clang-format on

// userspace state
typedef struct {
    // track the osl state of mods
    union {
        struct {
            uint8_t ctrl_os : 1;
            uint8_t shft_os : 1;
            uint8_t alt_os : 1;
            uint8_t supr_os : 1;
        };
        uint8_t os_mods;
    };

    // track the held state of mods
    union {
        struct {
            uint8_t ctrl_held : 1;
            uint8_t shft_held : 1;
            uint8_t alt_held : 1;
            uint8_t supr_held : 1;
        };
        uint8_t held_mods;
    };

    // tracks the actual state of mods
    union {
        struct {
            uint8_t ctrl_pressed : 1;
            uint8_t shft_pressed : 1;
            uint8_t alt_pressed : 1;
            uint8_t supr_pressed : 1;
        };
        uint8_t pressed_mods;
    };
    // track the actual state of hold_mods
    bool hold_mods;
} kbd_state_t;

kbd_state_t state;

static void update_mod(uint8_t mod, bool pressed) {
    if (pressed) {
        register_mods(mod);
    } else {
        unregister_mods(mod);
    }
}

static void update_mods(kbd_state_t *s) {
#ifdef CONSOLE_ENABLE
    // If console is enabled, it will print the matrix position and status of each key pressed
    uprintf("osl %x, held %x pressed %x\n", s->os_mods, s->held_mods, s->pressed_mods);
#endif
    update_mod(MOD_BIT_LCTRL, s->ctrl_held);
    update_mod(MOD_BIT_LSHIFT, s->shft_held);
    update_mod(MOD_BIT_LALT, s->alt_held);
    update_mod(MOD_BIT_LGUI, s->supr_held);
}

void _process_combo_event(kbd_state_t *s, uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case CMB_CTRL:
            if (pressed) {
                s->ctrl_os   = 1;
                s->ctrl_held = 1;
            } else {
                s->ctrl_held &= s->hold_mods;
            }
            s->ctrl_pressed = pressed ? 1 : 0;
            break;
        case CMB_SHFT:
            if (pressed) {
                s->shft_os   = 1;
                s->shft_held = 1;
            } else {
                s->shft_held &= s->hold_mods;
            }
            s->shft_pressed = pressed ? 1 : 0;
            break;
        case CMB_ALT:
            if (pressed) {
                s->alt_os   = 1;
                s->alt_held = 1;
            } else {
                s->alt_held &= s->hold_mods;
            }
            s->alt_pressed = pressed ? 1 : 0;
            break;
        case CMB_SUPR:
            if (pressed) {
                s->supr_held = 1;
            } else {
                s->supr_held &= s->hold_mods;
            }

            s->supr_pressed = pressed ? 1 : 0;
            break;
    }
    update_mods(&state);
}

#define RECORD_KEEP_PROCESSING true
#define RECORD_STOP_PROCESSING false

bool _process_record_user(kbd_state_t *s, uint16_t keycode, keyrecord_t *record) {
    // custom handlers, stop processing
    bool pressed = record->event.pressed;
    if (keycode == QK_USER_0) {
        if (s->held_mods | s->hold_mods) {
            // track the hold_mods key
            s->hold_mods = pressed;
            // on release, update the mods to their actual state
            s->ctrl_held = s->ctrl_pressed;
            s->shft_held = s->shft_pressed;
            s->alt_held  = s->alt_pressed;
            s->supr_held = s->supr_pressed;
        } else {
            s->shft_held = pressed ? 1 : 0;
            // s->shft_pressed = pressed ? 1 : 0;
        }
        update_mods(&state);
        return RECORD_STOP_PROCESSING;
    }

    // any key release clears the current one shot modifiers
    if (IS_BASIC_KEYCODE(keycode) && pressed) {
        add_mods(s->os_mods);
    }

    return RECORD_KEEP_PROCESSING;
}

void process_combo_event(uint16_t combo_index, bool pressed) {
#ifdef CONSOLE_ENABLE
    // If console is enabled, it will print the matrix position and status of each key pressed
    uprintf("KL: kc: %d, pressed: %u\n", combo_index, pressed);
#endif
    return _process_combo_event(&state, combo_index, pressed);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    // If console is enabled, it will print the matrix position and status of each key pressed
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return _process_record_user(&state, keycode, record);
}

void clear_os_mods(kbd_state_t *s) {
    del_mods(s->os_mods);
    s->os_mods = 0;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;
    if ((IS_BASIC_KEYCODE(keycode) || keycode == KC_ESC) && !pressed) {
        clear_os_mods(&state);
    }
}
