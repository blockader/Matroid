#include "matroid.h"

const int8_t handness[MATRIX_ROWS][MATRIX_COLS] = {
    {-1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {-1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, -2},
    {-1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, -2},
    {-1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -2},
    {-1, -1, -1, -1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

#define PERMUTE( \
  k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e, k0f, \
	k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, k1e, \
	k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, \
	k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, k3e, \
	k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a \
) { \
    {k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e}, \
    {k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, KC_NO}, \
    {k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d, KC_NO}, \
    {k30, k31 ,k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, k3c, k3d, KC_NO}, \
    {k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k1e, k2e, k3e, k0f},  \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_NORM_BASE] = PERMUTE(
        KC_HASH, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KEY_DANCE(DANCE_PGDN_BOTTOM), KEY_DANCE(DANCE_PGUP_TOP), KC_NO, KEY_DELETE_LINE, KEY_FORWARD_LAYER(LAYER_RACE_BASE),
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_RBRC, KC_TAB, KC_BSLS, KEY_FORWARD_LAYER(LAYER_CONTROL),
        KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KC_NO, KC_CAPS, KEY_FORWARD_LAYER(LAYER_LEGACY_BASE),
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, LCTL(KC_LEFT),
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_LPRN, KC_SPC, KC_RALT, KC_RGUI, KC_NO, KC_NO, LCTL(KC_RIGHT)),
    [LAYER_NORM_EXTENSION] = PERMUTE(
        KEY_FORWARD_LAYER(LAYER_WINDOW), LOPT(KC_SPC), LCTL(KC_SPC), KC_GRAVE, KC_NO, KC_NO, KC_NO, KC_NO, KC_ESC, LGUI(LSFT(KC_4)), KC_MPLY, KC_VOLD, KC_VOLU, KC_NO, LCTL(KC_C), KEY_FORWARD_LAYER(LAYER_RACE_BASE),
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR, KC_TILD, KEY_CREATE_PREVIOUS_LINE, KEY_FORWARD_LAYER(LAYER_CONTROL),
        KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), LALT(KC_LEFT), LALT(KC_RIGHT), KC_BSPC, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, LGUI(KC_V), KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KC_NO, KEY_DELETE_WORD, KEY_FORWARD_LAYER(LAYER_LEGACY_BASE),
        KEY_CREATE_NEXT_LINE, KC_LSFT, KC_AT, KC_DLR, KC_LPRN, KC_RPRN, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, LCTL(KC_LEFT),
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RPRN, KC_SPC, KC_RALT, KC_RGUI, KC_NO, RESET, LCTL(KC_RIGHT)),
    [LAYER_RACE_BASE] = PERMUTE(
        KC_HASH, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER,
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_RBRC, KC_NO, KC_BSLS, KC_NO,
        KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_NO, KC_CAPS, KC_NO,
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_LPRN, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [LAYER_RACE_EXTENSION] = PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_GRAVE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_0, KC_NO, KC_NO, KEY_DELETE_LINE, KEY_BACK_LAYER,
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR, KC_TILD, KC_BSLS, KC_NO,
        KC_NO, KC_A, KC_R, KC_BSPC, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_NO, KC_NO, KEY_DELETE_LINE, KC_NO,
        KC_ENT, KC_LSFT, KC_AT, KC_DLR, KC_LPRN, KC_RPRN, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_RPRN, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [LAYER_GAME_BASE] = PERMUTE(
        KC_HASH, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER,
        KC_LBRC, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_RBRC, KC_NO, KC_BSLS, KC_NO,
        KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_A, KC_R, KC_S, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_RACE_EXTENSION), KC_NO, KC_CAPS, KC_NO,
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_LPRN, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [LAYER_GAME_EXTENSION] = PERMUTE(
      KC_NO, KC_NO, KC_NO, KC_GRAVE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_0, KC_NO, KC_NO, KEY_DELETE_LINE, KEY_BACK_LAYER,
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR, KC_TILD, KC_BSLS, KC_NO,
        KC_NO, KC_A, KC_R, KC_BSPC, KC_T, KC_D, KC_H, KC_N, KC_E, KC_I, KC_O, KC_NO, KC_NO, KEY_DELETE_LINE, KC_NO,
        KC_ENT, KC_LSFT, KC_AT, KC_DLR, KC_LPRN, KC_RPRN, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_SPC, KC_RPRN, KC_SPC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [LAYER_LEGACY_BASE] = PERMUTE(
        KC_HASH, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KEY_DANCE(DANCE_PGDN_BOTTOM), KEY_DANCE(DANCE_PGUP_TOP), KC_NO, KEY_DELETE_LINE, KC_NO,
        KC_LBRC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_RBRC, KC_TAB, KC_BSLS, KEY_FORWARD_LAYER(LAYER_CONTROL),
        KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KC_NO, KC_CAPS, KEY_BACK_LAYER,
        KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, KC_NO, LCTL(KC_LEFT),
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_LPRN, KC_SPC, KC_RALT, KC_RGUI, KC_NO, KC_NO, LCTL(KC_RIGHT)),
    [LAYER_LEGACY_EXTENSION] = PERMUTE(     
        KEY_FORWARD_LAYER(LAYER_WINDOW), LOPT(KC_SPC), LCTL(KC_SPC), KC_GRAVE, KC_NO, KC_NO, KC_NO, KC_NO, KC_ESC, LGUI(LSFT(KC_4)), KC_MPLY, KC_VOLD, KC_VOLU, KC_NO, LCTL(KC_C), KC_NO,
        KC_LCBR, KC_CIRC, KC_AMPR, KC_PIPE, KC_QUES, KC_PERC, KC_PEQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR, KC_TILD, KEY_CREATE_PREVIOUS_LINE, KEY_FORWARD_LAYER(LAYER_CONTROL),
        KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), LALT(KC_LEFT), LALT(KC_RIGHT), KC_BSPC, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, LGUI(KC_V), KEY_FORWARD_LAYER(LAYER_LEGACY_EXTENSION), KC_NO, KEY_DELETE_WORD, KEY_BACK_LAYER,
        KEY_CREATE_NEXT_LINE, KC_LSFT, KC_AT, KC_DLR, KC_LPRN, KC_RPRN, KC_UNDS, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT, KC_NO, LCTL(KC_LEFT),
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RPRN, KC_SPC, KC_RALT, KC_RGUI, KC_NO, KC_NO, LCTL(KC_RIGHT)),
    [LAYER_CONTROL] = PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_INSERT_LINE_START, KEY_INSERT_HERE, KEY_INSERT_LINE_END, KC_NO, KC_NO, KC_NO, KEY_CREATE_PREVIOUS_LINE, KEY_BACK_LAYER,
        KEY_BACK_LAYER, LALT(KC_LEFT), LALT(KC_RIGHT), KC_BSPC, KEY_MOVE_LINE_STRAT, LCTL(KC_E), KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, LGUI(KC_V), KEY_BACK_LAYER, KC_NO, KEY_CREATE_NEXT_LINE, KC_NO,
        KEY_CUT_WORD, KC_LSFT, LGUI(KC_Z), KEY_CUT_SELECTION, LGUI(KC_C), LGUI(KC_V), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [LAYER_WINDOW] = PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [LAYER_DESKTOP] = PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, LCTL(KC_LEFT), KC_NO, KC_NO, LCTL(KC_RIGHT), KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KEY_BACK_LAYER,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
};
