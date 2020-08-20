#include "matroid.h"

#define KEYMAP_PERMUTE LAYOUT_ergodox_pretty

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_NORM_BASE] = KEYMAP_PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, LGUI(KC_DOWN), LGUI(KC_UP), KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO,
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_PGDN, KC_PGUP, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_LPRN,
        KC_LCBR, KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_QUOT,
        KC_ASTR, KC_Z, KC_X, KC_C, KC_V, KC_B, LGUI(LSFT(KC_4)), LOPT(KC_SPC), KC_K, KC_M, KC_COMM, KC_DOT, KC_PLUS, KC_AMPR,
        KEY_LEFT_COMMAND, KC_LCTL, KC_LALT, LCTL(KC_SPC), KC_TAB, KC_ENT, KC_CAPS, KC_RALT, KC_RCTL, KEY_RIGHT_COMMAND,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_SPC, KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KEY_FORWARD_LAYER(LAYER_WINDOW), KC_ESC, KC_RSFT, KC_BSPC
    ),
    [LAYER_NORM_EXTENSION] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_FORWARD_LAYER(LAYER_OS), KC_NO, LCTL(KC_C), KC_MPLY, KC_VOLD, KC_VOLU, KC_NO, KC_NO,
        KC_RBRC, KC_CIRC, KC_QUES, KC_PERC, KC_DLR, KC_HASH, KEY_FORWARD_LAYER(LAYER_RACE_BASE), KEY_FORWARD_LAYER(LAYER_LEGACY_BASE), KC_GRAVE, KC_BSLS, KC_TILD, KC_AT, KC_COLN, KC_RPRN,
        KC_RCBR, LALT(KC_LEFT), LALT(KC_RIGHT), KC_EQL, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_EXLM, KC_DQUO,
        KC_SLSH, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), KEY_PASTE, LGUI(KC_A), KC_NO, KC_NO, LGUI(KC_S), KC_UNDS, KC_LT, KC_GT, KC_MINUS, KC_PIPE,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_CREATE_NEXT_LINE, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_DELETE_WORD
    ),
    [LAYER_RACE_BASE] = KEYMAP_PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, KC_NO, KC_NO, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO,
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_NO, KC_NO, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_LPRN,
        KC_LCBR, KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_QUOT,
        KC_ASTR, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_NO, KC_NO, KC_K, KC_M, KC_COMM, KC_DOT, KC_PLUS, KC_AMPR,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_CAPS, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_SPC, KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_NO, KC_NO, KC_RSFT, KEY_DELETE_WORD
    ),
    [LAYER_RACE_EXTENSION] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MPLY, KC_VOLD, KC_VOLU, KC_NO, KC_NO,
        KC_RBRC, KC_CIRC, KC_QUES, KC_PERC, KC_DLR, KC_HASH, KEY_BACK_LAYER, KC_NO, KC_GRAVE, KC_BSLS, KC_TILD, KC_AT, KC_COLN, KC_RPRN,
        KC_RCBR, KC_NO, KC_NO, KC_EQL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_EXLM, KC_DQUO,
        KC_SLSH, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UNDS, KC_LT, KC_GT, KC_MINUS, KC_PIPE,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_LEGACY_BASE] = KEYMAP_PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, LGUI(KC_DOWN), LGUI(KC_UP), KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO,
        KC_LBRC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_PGDN, KC_PGUP, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LPRN,
        KC_LCBR, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_ASTR, KC_Z, KC_X, KC_C, KC_V, KC_B, LGUI(LSFT(KC_4)), LOPT(KC_SPC), KC_N, KC_M, KC_COMM, KC_DOT, KC_PLUS, KC_AMPR,
        KEY_LEFT_COMMAND, KC_LCTL, KC_LALT, LCTL(KC_SPC), KC_TAB, KC_ENT, KC_CAPS, KC_RALT, KC_RCTL, KEY_RIGHT_COMMAND,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_SPC, KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KEY_FORWARD_LAYER(LAYER_WINDOW), KC_ESC, KC_RSFT, KC_BSPC
    ),
    [LAYER_LEGACY_EXTENSION] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_FORWARD_LAYER(LAYER_OS), KC_NO, LCTL(KC_C), KC_MPLY, KC_VOLD, KC_VOLU, KC_NO, KC_NO,
        KC_RBRC, KC_CIRC, KC_QUES, KC_PERC, KC_DLR, KC_HASH, KC_NO, KEY_BACK_LAYER, KC_GRAVE, KC_BSLS, KC_TILD, KC_AT, KC_COLN, KC_RPRN,
        KC_RCBR, LALT(KC_LEFT), LALT(KC_RIGHT), KC_EQL, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_EXLM, KC_DQUO,
        KC_SLSH, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), KEY_PASTE, LGUI(KC_A), KC_NO, KC_NO, LGUI(KC_S), KC_UNDS, KC_LT, KC_GT, KC_MINUS, KC_PIPE,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_CREATE_NEXT_LINE, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_DELETE_WORD
    ),
    [LAYER_CONTROL] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_WINDOW] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LCTL(KC_LEFT), KC_NO, KC_NO, LCTL(KC_RIGHT), KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_OS] = KEYMAP_PERMUTE(
        KC_NO, KEY_SWITCH_OS(MACOS), KEY_SWITCH_OS(LINUX), KEY_SWITCH_OS(WINDOWS), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
};

const int8_t handness[MATRIX_ROWS][MATRIX_COLS] = KEYMAP_PERMUTE(
    -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, 1, 1, 1, 1, 1,
    -1, -1, 1, 1,
    -1, 1,
    -1, -1, -1, 1, 1, 1
);
