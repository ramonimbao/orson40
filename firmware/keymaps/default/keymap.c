/* Copyright 2019 Ramon Imbao
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
#include QMK_KEYBOARD_H

#include "quantum.h"

#define _QWERTY 0
#define _COLEMAK 1
#define _RAISE 2
#define _LOWER 3
#define _ADJUST 4

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    KC_LLEFT = SAFE_RANGE,
    KC_RRIGHT,
    QWERTY,
    COLEMAK,
    LOWER,
    RAISE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
        KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINUS,
        KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCOLON, KC_QUOTE,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMMA, KC_DOT, KC_SLASH, MT(MOD_RSFT, KC_ENTER),
        KC_LCTRL, KC_LALT, KC_LGUI, LOWER, KC_SPACE, KC_BSPACE, RAISE, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
    ),
    [_COLEMAK] = LAYOUT(
        KC_ESC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCOLON, KC_MINUS,
        KC_TAB, KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_QUOTE,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMMA, KC_DOT, KC_SLASH, MT(MOD_RSFT, KC_ENTER),
        KC_LCTRL, KC_LALT, KC_LGUI, LOWER, KC_SPACE, KC_BSPACE, RAISE, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT
    ),
    [_RAISE] = LAYOUT(
        KC_TILDE, KC_EXCLAIM, KC_AT, KC_HASH, KC_DOLLAR, KC_PERCENT, KC_CIRCUMFLEX, KC_AMPERSAND, KC_ASTERISK, KC_LPRN, KC_RPRN, KC_PLUS,
        KC_CAPS, KC_TRNS, KC_LT, KC_LCBR, KC_LPRN, KC_LBRC, KC_RBRC, KC_RPRN, KC_RCBR, KC_GT, KC_TRNS, KC_PIPE,
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL, KC_TRNS, KC_LLEFT, KC_TRNS, KC_TRNS, KC_RRIGHT
    ),
    [_LOWER] = LAYOUT(
        KC_GRAVE, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_EQUAL,
        KC_CAPS, KC_TRNS, KC_LT, KC_LCBR, KC_LPRN, KC_LBRC, KC_RBRC, KC_RPRN, KC_RCBR, KC_GT, KC_TRNS, KC_BSLASH,
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END
    ),
    [_ADJUST] = LAYOUT(
        QWERTY, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, COLEMAK,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LLEFT:
            if (record->event.pressed) {
                register_code16(LCTL(KC_LEFT));
            } else {
                unregister_code16(LCTL(KC_LEFT));
            }
            break;
        case KC_RRIGHT:
            if (record->event.pressed) {
                register_code16(LCTL(KC_RIGHT));
            } else {
                unregister_code16(LCTL(KC_RIGHT));
            }
            break;
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            break;
        case COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK);
            }
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
    }
    return true;
}

void matrix_init_user(void) {}

void matrix_scan_user(void) {}

void led_set_user(uint8_t usb_led) {}
