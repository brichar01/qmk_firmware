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
#include "quantum.h"
#include "quantum_keycodes.h"
#include "util.h"
#include QMK_KEYBOARD_H

#include "action.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[0] = LAYOUT_split_3x6_3(
                                                                KC_ESC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_BSPC, 
                                                                QK_USER_0, KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_QUOT, 
                                                                KC_TAB, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT, 
                                                                                                KC_LGUI, MO(2), KC_LSFT, KC_SPC, MO(1), KC_LALT),
                                                              [1] = LAYOUT_split_3x6_3(
                                                                TO(1), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), _______, 
                                                                _______, KC_TILD, KC_LCBR, KC_LBRC, KC_MINS, KC_EQL, _______, KC_PIPE, KC_GRV, _______, _______, QK_USER_1, 
                                                                A(KC_TAB), _______, KC_RCBR, KC_RBRC, _______, _______, _______, _______, _______, _______, KC_BSLS, QK_LEAD, 
                                                                                                         _______, MO(3), _______, _______, _______, _______),
                                                              [2] = LAYOUT_split_3x6_3(
                                                                _______, S(KC_B), S(KC_C), S(KC_D), S(KC_E), S(KC_F), _______, _______, _______, _______, _______, 
                                                                _______, _______, KC_6, KC_7, KC_8, KC_9, S(KC_A), KC_PAGE_UP, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, 
                                                                KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_PAGE_DOWN, KC_HOME, KC_END, _______, _______, QK_LEAD, 
                                                                                              _______, _______, _______, KC_LSFT, MO(3), _______),
                                                              [3] = LAYOUT_split_3x6_3(
                                                                KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, _______, _______, _______, _______, _______, KC_DEL, 
                                                                KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, _______, _______, _______, _______, 
                                                                _______, S(C(KC_Z)), _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_LEAD, 
                                                                                                          _______, _______, _______, _______, _______, _______)};
// clang-format on

// const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// // This globally defines all key overrides to be usedttxg
// const key_override_t *key_overrides[] = {}
// 	&delete_key_override
// };

// const uint16_t PROGMEM windows_key_combo[] = {KC_H, KC_D, COMBO_END};

// combo_t key_combos[] = {
//     COMBO(windows_key_combo, KC_LGUI),
// };

uint16_t i_was = KC_NO;
// fill the space missing when I try to use the other keys but the next key I want is on the same finger
bool process_qk_user_0(bool keydown) {
    if (!keydown) {
        unregister_code(i_was);
        return false;
    }
    // keydown
    if (get_mods() & MOD_BIT_LGUI) { // win+shift
        i_was = KC_LSFT;
        register_code(KC_LSFT);
    } else {
        i_was = KC_LCTL;
        register_code(KC_LCTL); // normally left ctrl
    }
    return false;
}

uint16_t keycode;

typedef struct {
    uint16_t *buff;
    size_t    len;
    size_t    ind;
} key_buffer_t;

// brace closer order (fifo buffer so we close patterns like [""])
static uint16_t     brace_st[2];
static key_buffer_t brace = {
    .buff = brace_st,
    .len  = sizeof(brace_st) / sizeof(brace_st[0]),
    .ind  = 0,
};

// Pressed buttons, in case another closable button is pressed
// while the closer is still pressed
static uint16_t     brace_outst[2];
static key_buffer_t brace_pressed = {
    .buff = brace_outst,
    .len  = sizeof(brace_outst) / sizeof(brace_outst[0]),
    .ind  = 0,
};

static uint16_t buff_pop(key_buffer_t *buff) {
    uint16_t val          = brace.buff[brace.ind];
    brace.buff[brace.ind] = KC_NO;
    brace.ind             = (brace.ind + 1) % 2;
    return val;
}

static void buff_push(key_buffer_t *buff, uint16_t val) {
    brace.buff[brace.ind] = val;
    brace.ind             = (brace.ind + 1) % 2;
}

bool process_qk_user_1(uint16_t keycode, bool key_down) {
    // throw the cheese
    if (keycode == QK_USER_1) {
        print("Throwing the cheese");
        if (key_down) {
            uint16_t key = buff_pop(&brace);
            register_code(key);
            buff_push(&brace_pressed, key);
        } else {
            unregister_code(buff_pop(&brace));
        }
        return false;
    }

    // fill donut buffer
    switch (keycode) {
        case KC_QUOT: {
            print("KC_QUOTE");
            buff_push(&brace_pressed, KC_QUOTE);
        } break;
        case KC_LBRC: {
            if (((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT)) {
                // this pleb used the shift button
                buff_push(&brace_pressed, KC_RCBR);
                break;
            }
            print("KC_RBRC");
            buff_push(&brace_pressed, KC_RBRC);
        } break;
        case KC_LCBR: {
            print("KC_RCBR");
            buff_push(&brace_pressed, KC_RCBR);
        } break;
        case KC_LPRN: {
            print("KC_RPRN");
            buff_push(&brace_pressed, KC_RPRN);
        }
        case KC_9: {
            if (!((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT)) {
                // these aren't the keypresses you're looking for
                return true;
            }
            print("KC_RPRN shifted manually");
            buff_push(&brace_pressed, KC_RPRN);
        } break;
        default:
            break;
    }
    return true;
}

typedef bool (*sequence_callback)(void);

typedef struct {
    const uint16_t   *sequence;
    sequence_callback callback;
    uint16_t          sequence_key;
} leader_sequence_t;

const uint16_t leader_logic_and[4]        = {KC_A, KC_N, KC_D};
const uint16_t leader_logic_for_all[4]    = {KC_A, KC_L, KC_L};
const uint16_t leader_logic_ifthen[4]     = {KC_I, KC_F, KC_T};
const uint16_t leader_logic_iffthen[4]    = {KC_I, KC_F, KC_F};
const uint16_t leader_logic_not_equal[4]  = {KC_N, KC_E, KC_Q};
const uint16_t leader_logic_not[4]        = {KC_N, KC_O, KC_T};
const uint16_t leader_logic_necessity[4]  = {KC_N, KC_E, KC_C};
const uint16_t leader_logic_modal_all[4]  = {KC_M, KC_A, KC_L, KC_L};
const uint16_t leader_logic_modal_some[4] = {KC_M, KC_S, KC_O, KC_M};
const uint16_t leader_logic_or[4]         = {KC_O, KC_R};
const uint16_t leader_logic_for_some[4]   = {KC_S, KC_O, KC_M, KC_E};
const uint16_t leader_logic_therefore[4]  = {KC_T, KC_H, KC_F};

typedef enum {
    LEADER_LOGIC_AND,
    LEADER_LOGIC_OR,
    LEADER_LOGIC_NOT,
    LEADER_LOGIC_IFTHEN,
    LEADER_LOGIC_IFFTHEN,
    LEADER_LOGIC_THEREFORE,
    LEADER_LOGIC_NECESSITY,
    LEADER_LOGIC_FOR_SOME,
    LEADER_LOGIC_FOR_ALL,
    LEADER_LOGIC_NOT_EQUAL,
    LEADER_LOGIC_MODAL_SOME,
    LEADER_LOGIC_MODAL_ALL,
} leader_class_logic_e;

// clang-format off
const leader_sequence_t leader_class_logic_sequences[] = {
    [LEADER_LOGIC_AND] = {.sequence = leader_logic_and, .sequence_key = 0x2227},
    [LEADER_LOGIC_OR] = {.sequence = leader_logic_or, .sequence_key = 0x2228}, 
    [LEADER_LOGIC_NOT] = {.sequence = leader_logic_not, .sequence_key = 0x00AC}, 
    [LEADER_LOGIC_IFTHEN] = {.sequence = leader_logic_ifthen, .sequence_key = 0x2192}, 
    [LEADER_LOGIC_IFFTHEN] = {.sequence = leader_logic_iffthen, .sequence_key = 0x2194}, 
    [LEADER_LOGIC_THEREFORE] = {.sequence = leader_logic_therefore, .sequence_key = 0x22A6}, 
    [LEADER_LOGIC_NECESSITY] = {.sequence = leader_logic_necessity, .sequence_key = 0x22A7}, 
    [LEADER_LOGIC_FOR_SOME] = {.sequence = leader_logic_for_some, .sequence_key = 0x2200}, 
    [LEADER_LOGIC_FOR_ALL] = {.sequence = leader_logic_for_all, .sequence_key = 0x2203}, 
    [LEADER_LOGIC_NOT_EQUAL] = {.sequence = leader_logic_not_equal, .sequence_key = 0x2227}, 
    [LEADER_LOGIC_MODAL_SOME] = {.sequence = leader_logic_modal_some, .sequence_key = 0x25C1}, 
    [LEADER_LOGIC_MODAL_ALL] = {.sequence = leader_logic_modal_all, .sequence_key = 0x25A1},
};
// clang-format on

typedef enum { LEADER_CLASS_NONE, LEADER_CLASS_LOGIC } leader_class_e; // first character of a leader sequence is its memonic class

typedef struct {
    uint16_t                 key;
    const leader_sequence_t *sequences;
    const uint16_t           num_sequences;
} leader_class_t;

#define LEADER_CLASS(kc, x) \
    { kc, x, ARRAY_SIZE(x) }

const leader_class_t leader_classes[] = {
    [LEADER_CLASS_LOGIC] = LEADER_CLASS(KC_L, leader_class_logic_sequences),
};

static const leader_class_t    *active_leader_class = NULL;
static bool                     sequence_active[ARRAY_SIZE(leader_class_logic_sequences)];
static leader_sequence_t const *winner;
bool                            did_finish_seq = false;
extern uint8_t                  leader_sequence_size;

void leader_start_user(void) {
    uprint("Leader start\n");
    for (int i = 0; i < ARRAY_SIZE(leader_class_logic_sequences); i++) {
        sequence_active[i] = true;
    }
    did_finish_seq      = false;
    active_leader_class = NULL;
    winner              = NULL;
}

bool leader_add_user(uint16_t keycode) {
    // first letter refers to what leader sequence set to use
    if (active_leader_class == NULL) {
        for (int i = 0; i < ARRAY_SIZE(leader_classes); i++) {
            if (keycode == leader_classes[i].key) {
                uprintf("leader selected: %i\n", i);
                active_leader_class = &leader_classes[i];
                return false;
            }
        }
        return false;
    }
    uint8_t num_active_sequences = 0;
    // match leader and return true only one option remains
    for (int i = 0; i < active_leader_class->num_sequences; i++) {
        if (sequence_active[i]) {
            sequence_active[i] = active_leader_class->sequences[i].sequence[leader_sequence_size - 2] == keycode;
            if (sequence_active[i]) {
                if (active_leader_class->sequences[i].sequence[leader_sequence_size - 1] == 0 || leader_sequence_size >= 5) {
                    winner = &active_leader_class->sequences[i];
                }
                num_active_sequences++;
            }
        }
    }
    uprintf("Num sequences remaining: %i\n", num_active_sequences);
    return winner != NULL;
}

void leader_end_user(void) {
    // act on leader completion
    uprintf("Ended, did_finish_seq %i, key %x\n", did_finish_seq, winner->sequence_key);
    if (winner != NULL) {
        if (winner->sequence_key != KC_NO) {
            register_unicode(winner->sequence_key);
        } else if (winner->callback != NULL) {
            winner->callback();
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    // If console is enabled, it will print the matrix position and status of each key pressed
    // uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    // always run section (for tracking other keys)
    bool keep_prc = process_qk_user_1(keycode, record->event.pressed);
    if (!keep_prc) {
        return keep_prc;
    }

    // run on keypress section (only cares about itself)
    if (keycode == QK_USER_0) {
        return process_qk_user_0(record->event.pressed);
    }
    return true;
}
