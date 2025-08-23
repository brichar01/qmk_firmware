/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

//#define USE_MATRIX_I2C

//#define QUICK_TAP_TERM 0
//#define TAPPING_TERM 100

// #define DEBUG_MATRIX_SCAN_RATE

#define NO_ALT_REPEAT_KEY
#define DEBOUNCE 50 // 50 presses per second, that should be plenty
#define UNICODE_KEY_LNX LCTL(LSFT(KC_U))
#define UNICODE_KEY_WINC KC_RIGHT_ALT
#define UNICODE_CYCLE_PERSIST true
#define UNICODE_TYPE_DELAY 10
#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX, UNICODE_MODE_WINCOMPOSE
#define LEADER_TIMEOUT 750
#define LEADER_PER_KEY_TIMING
#define LEADER_NO_TIMEOUT

#ifdef RGB_MATRIX_ENABLE
// #   define RGB_MATRIX_KEYPRESSES // reacts to keypresses
// #   define RGB_MATRIX_KEYRELEASES // reacts to keyreleases (instead of keypresses)
// #   define RGB_MATRIX_SLEEP // turn off effects when suspended
#   define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#   define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
#   define RGB_MATRIX_LED_FLUSH_LIMIT 200 // limits in milliseconds how frequently an animation will update the LEDs. 016 (16ms) is equivalent to limiting to 6fps (increases keyboard responsiveness)
#    define RGB_MATRIX_HUE_STEP 8
#    define RGB_MATRIX_SAT_STEP 8
#    define RGB_MATRIX_VAL_STEP 8
#    define RGB_MATRIX_SPD_STEP 5
#    define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#endif

