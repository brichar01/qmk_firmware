/* Copyright 2017 Fredric Silberberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "process_key_lock.h"
#include "keycodes.h"
#include "action.h"
#include "debug.h"

#define MOD_KEY_MIN KC_LEFT_CTRL
#define MOD_KEY_MAX KC_RIGHT_GUI
#define MOD_KEY_LEN (KC_RIGHT_GUI - KC_LEFT_CTRL)

// initial modifier state
static bool real_mod_state[MOD_KEY_LEN] = {};
// Locked modifiers state
static bool mod_state[MOD_KEY_LEN] = {};
// watch while the lock key is pressed for new mod key presses
static bool watching = false;
// lock/unlock modifiers
static bool locked = false;

/*
 * Modifier locker code
 * 
 * Hold modifier(s) and the lock key and the modifier will remain locked 
 * until the lock key is pressed again.

 * TODO: Handle soft mods/etc better, I don't use them yet.
 *   * See what other defines are going to mess with this.
 */
bool process_key_lock(uint16_t *keycode, keyrecord_t *record) {
    bool key_down = record->event.pressed;
    bool key_up   = !record->event.pressed;

    // maintain list of active modifier keys
    if(IS_MODIFIER_KEYCODE(*keycode)) {
        dprint("Pressed modifier while not watching");
        real_mod_state[*keycode - MOD_KEY_MIN] = key_down;
        if(!watching && !locked) {
            // no behaviour to modify, early return
            return true;
        }
    }
    
    // start the process of locking mods
    if (*keycode == QK_LOCK) {
        dprint("Pressed keylock\n");
        // re-press to kill all locks and start scanning for new locks
        if (key_down) {
            dprint("New keylock\n");
            locked = false;
            watching = true;
            for (int i = 0; i < MOD_KEY_LEN; i++) {
                if (mod_state[i] && !real_mod_state[i]) { // deregister any modifiers in the locked group that are not actually being pressed
                    unregister_code(MOD_KEY_MIN + i);
                }
            }
            for (int i = 0; i < MOD_KEY_LEN; i++) {
                mod_state[i] = real_mod_state[i];
            }
            return false;
        } else { // key_up, lock/unlock the modifiers
            dprint("Released keylock key\n");
            for (int i = 0; i < MOD_KEY_LEN; i++) { // if any modifiers are pressed, lock them
                if (mod_state[i]) {
                    locked = true;
                    break;
                }
            }
            watching = false;
        }
        return false; // no more proccessing for this keycode
    }

    // handle modifiers based on state
    if (IS_MODIFIER_KEYCODE(*keycode)) {
        // kill the correct modifier keyups while locked
        if (key_up && mod_state[*keycode - MOD_KEY_MIN]) {
            dprint("Killed a modifier keyup\n");
            return false;
        }

        // update pressed mods while we are watching
        if (watching && key_down) {
            dprint("Adding a new mod to the locked list\n");
            mod_state[*keycode - MOD_KEY_MIN] = true;
            return true;
        }
    }
    return true;
}