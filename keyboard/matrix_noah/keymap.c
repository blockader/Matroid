#include "matroid.h"

#define KEYMAP_PERMUTE( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0e, k0f, \
	k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, k1e, \
	k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2d, k2e, \
	k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, k3e, \
	k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a \
) { \
    {k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, KC_SPC, k0e}, \
    {k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, KC_NO}, \
    {k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, KC_NO, k2d, KC_NO}, \
    {k30, k31 ,k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, KC_NO}, \
    {k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k1e, k2e, k3e, k0f}, \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_NORM_BASE] = KEYMAP_PERMUTE(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KEY_DANCE(DANCE_PGDN_BOTTOM), KEY_DANCE(DANCE_PGUP_TOP), KC_NO, KEY_FORWARD_LAYER(LAYER_RACE_BASE),
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_LPRN, KC_NO, KC_NO, KEY_FORWARD_LAYER(LAYER_LEGACY_BASE),
        TRAIT_LEFT(KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION)), KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, TRAIT_RIGHT(KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION)), KC_CAPS, KEY_FORWARD_LAYER(LAYER_OS),
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, KC_NO,
        KC_LCTL, KEY_LEFT_COMMAND, KC_LALT, TRAIT_LEFT(KC_SPC), KC_SPC, TRAIT_RIGHT(KC_SPC), KC_RALT, KEY_RIGHT_COMMAND, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_NORM_EXTENSION] = KEYMAP_PERMUTE(
        KEY_FORWARD_LAYER(LAYER_WINDOW), LOPT(KC_SPC), LCTL(KC_SPC), KC_DLR, KC_HASH, KC_GRAVE, KC_BSLS, KC_TILD, KC_AT, LGUI(LSFT(KC_4)), KC_MPLY, KC_VOLD, KC_VOLU, LCTL(KC_C), KEY_FORWARD_LAYER(LAYER_RACE_BASE),
        KC_RBRC, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_EQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RPRN, KC_NO, KC_NO, KEY_FORWARD_LAYER(LAYER_LEGACY_BASE),
        KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), LALT(KC_LEFT), LALT(KC_RIGHT), KC_BSPC, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KEY_PASTE, KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KC_CAPS, KEY_FORWARD_LAYER(LAYER_OS),
        KEY_CREATE_NEXT_LINE, KC_LSFT, KC_TAB, KEY_DELETE_WORD, KC_LCBR, KC_RCBR, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, KC_NO,
        KC_LCTL, KEY_LEFT_COMMAND, KC_LALT, KC_SPC, KC_SPC, KC_SPC, KC_RALT, KEY_RIGHT_COMMAND, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_RACE_BASE] = KEYMAP_PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER,
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_LPRN, KC_NO, KC_NO, KC_NO,
        KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_CAPS, KC_NO,
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_SPC, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_RACE_EXTENSION] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_DLR, KC_HASH, KC_GRAVE, KC_BSLS, KC_TILD, KC_AT, KC_NO, KC_NO, KC_0, KC_NO, KC_NO, KEY_BACK_LAYER,
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RPRN, KC_NO, KC_NO, KC_NO,
        KC_RBRC, KC_A, KC_R, KC_BSPC, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KEY_DELETE_LINE, KC_NO,
        KC_ENT, KC_LSFT, KC_NO, KEY_DELETE_LINE, KC_LCBR, KC_RCBR, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_SPC, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_GAME_BASE] = KEYMAP_PERMUTE( // TODO: Not implemented.
        KC_HASH, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_RBRC, KC_NO, KC_NO, KEY_BACK_LAYER,
        KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION), KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION), KC_CAPS, KC_NO,
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_LPRN, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_GAME_EXTENSION] = KEYMAP_PERMUTE( // TODO: Not implemented.
        KC_NO, KC_NO, KC_NO, KC_NO, KC_GRAVE, KC_BSLS, KC_NO, KEY_DELETE_LINE, KC_NO, KC_NO, KC_NO, KC_0, KC_NO, KC_NO, KC_NO,
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR, KC_TILD, KC_BSLS, KEY_BACK_LAYER,
        KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION), KC_A, KC_R, KC_BSPC, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION), KEY_DELETE_LINE, KC_NO,
        KC_ENT, KC_LSFT, KC_AT, KC_DLR, KC_LPRN, KC_RPRN, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_RPRN, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_LEGACY_BASE] = KEYMAP_PERMUTE( // TODO: Should be renamed to QWERTY and have common symbol positions.
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KEY_DANCE(DANCE_PGDN_BOTTOM), KEY_DANCE(DANCE_PGUP_TOP), KC_NO, KC_NO,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_RBRC, KC_TAB, KC_NO, KEY_BACK_LAYER,
        KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KC_CAPS, KEY_FORWARD_LAYER(LAYER_OS),
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, LCTL(KC_LEFT),
        KC_LCTL, KEY_LEFT_COMMAND, KC_LALT, KC_SPC, KC_SPC, KC_SPC, KC_RALT, KEY_RIGHT_COMMAND, KC_NO, KC_NO, LCTL(KC_RIGHT)
    ),
    [LAYER_LEGACY_EXTENSION] = KEYMAP_PERMUTE( // TODO: Should be deleted.
        KEY_FORWARD_LAYER(LAYER_WINDOW), LOPT(KC_SPC), LCTL(KC_SPC), KC_NO, KC_GRAVE, KC_BSLS, KEY_CREATE_PREVIOUS_LINE, KEY_DELETE_LINE, KC_ESC, LGUI(LSFT(KC_4)), KC_MPLY, KC_VOLD, KC_VOLU, LCTL(KC_C), KC_NO,
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR, KC_TILD, KC_NO, KEY_BACK_LAYER,
        KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), LALT(KC_LEFT), LALT(KC_RIGHT), KC_BSPC, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, LGUI(KC_V), KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KEY_DELETE_WORD, KEY_FORWARD_LAYER(LAYER_OS),
        KEY_CREATE_NEXT_LINE, KC_LSFT, KC_AT, KC_DLR, KC_LPRN, KC_RPRN, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, LCTL(KC_LEFT),
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RPRN, KC_SPC, KC_RALT, KC_RGUI, KC_NO, KC_NO, LCTL(KC_RIGHT)
    ),
    [LAYER_CONTROL] = KEYMAP_PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_INSERT_LINE_START, KEY_INSERT_HERE, KEY_INSERT_LINE_END, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, LALT(KC_LEFT), LALT(KC_RIGHT), KC_BSPC, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, LGUI(KC_V), KC_NO, KEY_CUT_WORD, KC_NO,
        KEY_CREATE_NEXT_LINE, KC_LSFT, LGUI(KC_Z), KEY_CUT_SELECTION, LGUI(KC_C), LGUI(KC_V), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_WINDOW] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_NO, KC_NO, KC_NO, KC_NO,
        KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO, KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_DESKTOP] = KEYMAP_PERMUTE( // TODO: May be should be deleted because of lack of use.
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LCTL(KC_LEFT), KC_NO, KC_NO, LCTL(KC_RIGHT), KC_NO, KC_NO, KC_NO, KC_NO,
        KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO, KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [LAYER_OS] = KEYMAP_PERMUTE(
        KC_NO, KEY_SWITCH_OS(MACOS), KEY_SWITCH_OS(LINUX), KEY_SWITCH_OS(WINDOWS), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
};

#define HANDEDNESS_PERMUTE( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0e, k0f, \
	k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, k1e, \
	k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2d, k2e, \
	k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, k3e, \
	k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a \
) { \
    {k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, 0, k0e}, \
    {k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, 0}, \
    {k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, 0, k2d, 0}, \
    {k30, k31 ,k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, 0}, \
    {k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k1e, k2e, k3e, k0f}, \
}

const int8_t handness[MATRIX_ROWS][MATRIX_COLS] = HANDEDNESS_PERMUTE(
    -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1,
    -1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 1
);
