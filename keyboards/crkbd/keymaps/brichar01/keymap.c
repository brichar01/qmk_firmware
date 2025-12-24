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
#include "action_layer.h"
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
                                                                KC_ESC, QK_USER_1, C(KC_X), C(KC_C), C(KC_V), QK_REP, KC_ENT, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_PRINT_SCREEN,
                                                                _______, C(KC_Z), _______, _______, _______, KC_LGUI, KC_PAGE_DOWN, KC_BSPC, KC_DEL, KC_TAB, QK_MOUSE_BUTTON_2, _______,
                                                                _______, _______, MO(4), MO(4), _______, _______),
                                                              [2] = LAYOUT_split_3x6_3(
                                                                _______, _______, KC_AT, KC_HASH, KC_DLR, KC_PERC, _______, KC_GRAVE, KC_CIRC, KC_COLON, _______, _______,
                                                                KC_EXLM, KC_TILDE, KC_AMPERSAND, KC_PLUS, KC_MINUS, _______, _______, KC_SEMICOLON, KC_EQL, KC_ASTR, KC_UNDERSCORE, _______,
                                                                _______, _______, _______, KC_BACKSLASH, KC_PIPE, KC_EQL, _______, _______, _______, _______, _______, _______,
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
                                                              [5] = LAYOUT_split_3x6_3(
                                                                XXXXXXX, XXXXXXX, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, XXXXXXX, XXXXXXX,
                                                                KC_Q, KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_QUOTE,
                                                                XXXXXXX, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, XXXXXXX,
                                                                XXXXXXX, MO(6), KC_LEFT_SHIFT, KC_SPC, KC_ENT, XXXXXXX),
                                                              // [6] =
};
// clang-format on

#define SEND_KEY(pressed, keycode)    \
    do {                              \
        if (pressed) {                \
            register_code(keycode);   \
        } else {                      \
            unregister_code(keycode); \
        }                             \
    } while (false)

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

const uint16_t PROGMEM cmb_lprn[]  = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM cmb_rprn[]  = {KC_M, KC_COMMA, COMBO_END};
const uint16_t PROGMEM cmb_lbrc[]  = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM cmb_rbrc[]  = {KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM cmb_lcbrc[] = {KC_V, KC_X, COMBO_END};
const uint16_t PROGMEM cmb_rcbrc[] = {KC_M, KC_DOT, COMBO_END};
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

typedef union {
    struct {
        uint8_t ctrl : 1;
        uint8_t shft : 1;
        uint8_t alt : 1;
        uint8_t supr : 1;
    };
    uint8_t mods;
} mods_t;

typedef enum {
    ALT_TAB_IDLE,
    ALT_TAB_TABBING,
} alt_tab_state_e;

// userspace state
typedef struct {
    // track the osl state of mods
    mods_t os;
    // track the held state of mods
    mods_t held;
    // track the actual state of hold_mods
    mods_t pressed;
    // set by shift to freez held mods (pressed continues to update and is restored when shift is released)
    bool hold_mods;

    alt_tab_state_e alt_tab_st;
} kbd_state_t;

kbd_state_t state;

static void update_mod(uint8_t mod, bool pressed) {
    if (pressed) {
        register_mods(mod);
    } else {
        unregister_mods(mod);
    }
}

static void send_updated_mods(mods_t *mods) {
#ifdef CONSOLE_ENABLE
    // If console is enabled, it will print the matrix position and status of each key pressed
    uprintf("mods %x\n", mods->mods);
#endif
    update_mod(MOD_BIT_LCTRL, mods->ctrl);
    update_mod(MOD_BIT_LSHIFT, mods->shft);
    update_mod(MOD_BIT_LALT, mods->alt);
    update_mod(MOD_BIT_LGUI, mods->supr);
}

void _process_combo_event(kbd_state_t *s, uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case CMB_CTRL:
            if (pressed) {
                s->os.ctrl   = 1;
                s->held.ctrl = 1;
            } else {
                s->held.ctrl &= s->hold_mods;
            }
            s->pressed.ctrl = pressed ? 1 : 0;
            break;
        case CMB_SHFT:
            if (pressed) {
                s->os.shft   = 1;
                s->held.shft = 1;
            } else {
                s->held.shft &= s->hold_mods;
            }
            s->pressed.shft = pressed ? 1 : 0;
            break;
        case CMB_ALT:
            if (pressed) {
                s->os.alt   = 1;
                s->held.alt = 1;
            } else {
                s->held.alt &= s->hold_mods;
            }
            s->pressed.alt = pressed ? 1 : 0;
            break;
        case CMB_SUPR:
            if (pressed) {
                s->held.supr = 1;
            } else {
                s->held.supr &= s->hold_mods;
            }
            s->pressed.supr = pressed ? 1 : 0;
            break;
    }
    send_updated_mods(&s->held);
}

#define RECORD_KEEP_PROCESSING true
#define RECORD_STOP_PROCESSING false

bool handle_alt_tab(kbd_state_t *s, uint16_t keycode, bool pressed) {
    if (keycode != QK_USER_1 && keycode != QK_TRI_LAYER_LOWER) {
        // no work to do (but really, don't call it)
#ifdef CONSOLE_ENABLE
        printf("Called with invalid keycode %x\n", keycode);
#endif
        return RECORD_KEEP_PROCESSING;
    }

#ifdef CONSOLE_ENABLE
    printf("State %s, key %x, pressed: %i\n", s->alt_tab_st ? "Tabbing" : "Idle", keycode, pressed);
#endif
    switch (s->alt_tab_st) {
        case ALT_TAB_IDLE: {
            if (keycode == QK_USER_1 && pressed) {
                s->alt_tab_st = ALT_TAB_TABBING;
                s->held.alt   = 1;
                send_updated_mods(&s->held);
                register_code(KC_TAB);
            }
            if (keycode == QK_TRI_LAYER_LOWER) {
                return RECORD_KEEP_PROCESSING;
            }
        } break;
        case ALT_TAB_TABBING: {
            // this catches the relase of the entering keystroke also
            if (keycode == QK_USER_1) {
                SEND_KEY(pressed, KC_TAB);
            }
            // releasing the layer releases alt
            if (keycode == QK_TRI_LAYER_LOWER && !pressed) {
                s->alt_tab_st = ALT_TAB_IDLE;
                s->held.alt   = 0;
                send_updated_mods(&s->held);
                return RECORD_KEEP_PROCESSING;
            }
        }
    }

    return RECORD_STOP_PROCESSING;
}

bool _process_record_user(kbd_state_t *s, uint16_t keycode, keyrecord_t *record) {
    // custom handlers, stop processing
    bool pressed = record->event.pressed;
    if (keycode == QK_USER_0) {
        if (s->held.mods | s->hold_mods) {
            // track the hold_mods key
            s->hold_mods = pressed;
            // on release, update the mods to their actual state
            s->held.ctrl = s->pressed.ctrl;
            s->held.shft = s->pressed.shft;
            s->held.alt  = s->pressed.alt;
            s->held.supr = s->pressed.supr;
        } else {
            s->held.shft = pressed ? 1 : 0;
            // s->shft_pressed = pressed ? 1 : 0;
        }
        send_updated_mods(&s->held);
        return RECORD_STOP_PROCESSING;
    }

    if (keycode == QK_TRI_LAYER_LOWER || keycode == QK_USER_1) {
        return handle_alt_tab(&state, keycode, pressed);
    }

    // any key release clears the current one shot modifiers
    if (IS_BASIC_KEYCODE(keycode) && pressed) {
        add_mods(s->os.mods);
    }

    return RECORD_KEEP_PROCESSING;
}

void process_combo_event(uint16_t combo_index, bool pressed) {
#ifdef CONSOLE_ENABLE
    // If console is enabled, it will rint the matrix position and status of each key pressed
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
    s->os.mods = 0;
    send_updated_mods(&s->held);
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;
    if ((IS_BASIC_KEYCODE(keycode) || keycode == KC_ESC) && !pressed && state.os.mods != 0) {
        clear_os_mods(&state);
    }
}
