#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "usb_descriptor.h"
#include <stdio.h>
#include <string.h>

enum {
    LAYER_NORM_BASE,
    LAYER_NORM_EXTENSION,
    LAYER_GAME_BASE,
    LAYER_GAME_EXTENSION,
    LAYER_OS,
    LAYER_FUNCTION,
    LAYER_WINDOW,
    LAYER_UTILITY,
    NUMBER_OF_LAYERS,
};

bool temporary[NUMBER_OF_LAYERS] = {
    [LAYER_NORM_BASE] = false, [LAYER_NORM_EXTENSION] = true,
    [LAYER_GAME_BASE] = false, [LAYER_GAME_EXTENSION] = true,
    [LAYER_OS] = true,         [LAYER_FUNCTION] = true,
    [LAYER_WINDOW] = true,     [LAYER_UTILITY] = true,
};

uint8_t layers[16];

enum {
    MACOS,
    LINUX,
    WINDOWS,
    NUMBER_OF_OSS,
};

enum {
    DEFAULT,
    TERMINAL,
    NUMBER_OF_APPLICATIONS,
};

struct {
    uint8_t os, application;
    uint16_t last_repeat_key;
    int last_repeat_time, last_repeat_interval;
    keyrecord_t last_repeat_record;
} common_layer_data;

struct {
    bool tab_disabled;
} layer_norm_extension_data;

struct {
    int start_time;
} layer_window_data;

#define KEY_FORWARD_LAYER(a) SAFE_RANGE + a
#define KEY_SWITCH_OS(a) SAFE_RANGE + NUMBER_OF_LAYERS + a

enum {
    DANCE_PGDN_BOTTOM,
    DANCE_PGUP_TOP,
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [DANCE_PGDN_BOTTOM] = ACTION_TAP_DANCE_DOUBLE(KC_PGDN, LGUI(KC_DOWN)),
    [DANCE_PGUP_TOP] = ACTION_TAP_DANCE_DOUBLE(KC_PGUP, LGUI(KC_UP)),
};

#define KEY_DANCE(a) TD(a)

enum {
    KEY_BACK_LAYER = SAFE_RANGE + NUMBER_OF_LAYERS + NUMBER_OF_OSS,
    KEY_LEFT_COMMAND,
    KEY_RIGHT_COMMAND,
    KEY_LEFT_CONTROL,
    KEY_RIGHT_CONTROL,
    KEY_MOVE_PREVIOUS_WORD,
    KEY_MOVE_NEXT_WORD,
    KEY_MOVE_LINE_STRAT,
    KEY_MOVE_LINE_END,
    KEY_DELETE_WORD,
    KEY_DELETE_LINE,
    KEY_CREATE_NEXT_LINE,
    KEY_CREATE_CPP_STATEMENT,
    KEY_CREATE_CPP_BODY,
    KEY_CREATE_LATEX_LEFT,
    KEY_CREATE_LATEX_RIGHT,
    NUMBER_OF_KEYS,
};

void keyboard_post_init_user() { rgblight_disable_noeeprom(); }

struct message {
    int32_t session_id;
    char *command, *arguments;
};

char send_message_buffer[RAW_EPSIZE + 1];

void send_message(const struct message *m) {
    memset(send_message_buffer, 0, RAW_EPSIZE + 1);
    if (m->arguments[0])
        sprintf(send_message_buffer, "%ld %s %s", m->session_id, m->command,
                m->arguments);
    else
        sprintf(send_message_buffer, "%ld %s", m->session_id, m->command);
    raw_hid_send((uint8_t *)send_message_buffer, RAW_EPSIZE);
}

void send_confusion(struct message *m) {
    m->command = "confusion";
    m->arguments[0] = 0;
    send_message(m);
}

void handle_message(struct message *m) {
    if (!strcmp(m->command, "heartbeat"))
        send_message(m);
    else if (!strcmp(m->command, "application")) {
        int v;
        if (sscanf(m->arguments, "%d", &v) != 1 || v < 0 ||
            v >= NUMBER_OF_APPLICATIONS) {
            send_confusion(m);
        } else {
            common_layer_data.application = v;
            m->arguments = "success";
            send_message(m);
        }
    } else
        send_confusion(m);
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    struct message m;
    char a[RAW_EPSIZE + 1];
    m.session_id = 0;
    m.arguments = a;
    int p = 0;
    if (data[p] < '0' || data[p] > '9') {
        send_confusion(&m);
        return;
    }
    for (; data[p] && data[p] >= '0' && data[p] <= '9'; ++p)
        m.session_id = m.session_id * 10 + (data[p] - '0');
    if (!data[p] || data[p] != ' ') {
        send_confusion(&m);
        return;
    }
    for (; data[p] && data[p] == ' '; ++p)
        ;
    if (!data[p]) {
        send_confusion(&m);
        return;
    }
    m.command = (char *)data + p;
    for (; data[p] && data[p] != ' '; ++p)
        ;
    if (data[p])
        data[p++] = 0;
    for (; data[p] && data[p] == ' '; ++p)
        ;
    strcpy(a, (char *)data + p);
    handle_message(&m);
}

bool handle_layer_key(uint16_t key, keyrecord_t *record) {
    switch (layers[layers[0] + 1]) {
    case LAYER_NORM_EXTENSION:
        switch (key) {
        case KC_QUES:
        case KC_MINUS:
        case KC_EXLM:
        case KC_COLN:
        case KC_DQUO:
            if (record->event.pressed)
                layer_norm_extension_data.tab_disabled = true;
            return true;
        case KC_TAB:
            if (record->event.pressed &&
                layer_norm_extension_data.tab_disabled) {
                tap_code(KC_SPC);
                return false;
            }
            return true;
        }
        return true;
    case LAYER_WINDOW:
        switch (key) {
        case SAFE_RANGE + LAYER_WINDOW:
            if (!record->event.pressed) {
                if (timer_elapsed(layer_window_data.start_time) < 200)
                    temporary[LAYER_WINDOW] = true;
            }
            return true;
        case KEY_BACK_LAYER:
            if (record->event.pressed)
                tap_code(KC_ENTER);
            return true;
        }
        return true;
    }
    return true;
}

void handle_layer_start(void) {
    switch (layers[layers[0] + 1]) {
    case LAYER_NORM_EXTENSION:
        layer_norm_extension_data.tab_disabled = false;
        return;
    case LAYER_WINDOW:
        register_code(KC_LGUI);
        tap_code(KC_TAB);
        layer_window_data.start_time = timer_read();
        return;
    }
}

void handle_layer_end(void) {
    switch (layers[layers[0] + 1]) {
    case LAYER_WINDOW:
        unregister_code(KC_LGUI);
        temporary[LAYER_WINDOW] = false;
        return;
    }
}

void update_layer(void) {
    layer_clear();
    layer_on(layers[layers[0] + 1]);
}

bool handle_call_key(uint16_t key, keyrecord_t *record) {
    switch (key) {
    case KEY_BACK_LAYER:
        if (record->event.pressed) {
            if (layers[0]) {
                if (temporary[layers[layers[0] + 1] && layers[0] > 1]) {
                    --layers[0];
                    handle_layer_end();
                    --layers[0];
                    ++layers[0];
                    layers[layers[0] + 1] = layers[layers[0] + 2];
                } else {
                    handle_layer_end();
                    --layers[0];
                    update_layer();
                }
            }
        }
        return false;
    }
    if (key >= SAFE_RANGE && key < SAFE_RANGE + NUMBER_OF_LAYERS) {
        uint8_t new_layer = key - SAFE_RANGE;
        if (record->event.pressed) {
            if (layers[layers[0] + 1] != new_layer) {
                if (temporary[layers[layers[0] + 1]]) {
                    handle_layer_end();
                    layers[layers[0] + 1] = new_layer;
                } else
                    layers[++layers[0] + 1] = new_layer;
                update_layer();
                handle_layer_start();
            }
        } else {
            if (layers[layers[0] + 1] == new_layer &&
                temporary[layers[layers[0] + 1]]) {
                handle_layer_end();
                --layers[0];
                update_layer();
            }
        }
        return false;
    }
    return true;
}

bool repeatable(uint16_t key) {
    switch (key) {
    case KC_LEFT:
    case KC_DOWN:
    case KC_UP:
    case KC_RIGHT:
    case KC_BSPC:
    case KC_ENT:
    case KC_TAB:
    case KEY_MOVE_PREVIOUS_WORD:
    case KEY_MOVE_NEXT_WORD:
    case KEY_DELETE_WORD:
        return true;
    case KC_C:
        return get_mods() && (get_mods() & MOD_MASK_CTRL) == get_mods();
    case KC_X:
    case KC_V:
        switch (common_layer_data.os) {
        case MACOS:
            return get_mods() && (get_mods() & MOD_MASK_GUI) == get_mods();
        case LINUX:
        case WINDOWS:
            return get_mods() && (get_mods() & MOD_MASK_CTRL) == get_mods();
        }
        return false;
    case KC_Z:
        switch (common_layer_data.os) {
        case MACOS:
            return get_mods() & ~MOD_MASK_SHIFT &&
                   (get_mods() & ~MOD_MASK_SHIFT & MOD_MASK_GUI) ==
                       (get_mods() & ~MOD_MASK_SHIFT);
        case LINUX:
        case WINDOWS:
            return get_mods() & ~MOD_MASK_SHIFT &&
                   (get_mods() & ~MOD_MASK_SHIFT & MOD_MASK_CTRL) ==
                       (get_mods() & ~MOD_MASK_SHIFT);
        }
        return false;
    }
    return false;
}

bool handle_repeat_key(uint16_t key, keyrecord_t *record) {
    if (layers[layers[0] + 1] == LAYER_GAME_BASE ||
        layers[layers[0] + 1] == LAYER_GAME_EXTENSION)
        return true;
    if (record->event.pressed) {
        if (repeatable(key) && !common_layer_data.last_repeat_key) {
            common_layer_data.last_repeat_key = key;
            common_layer_data.last_repeat_time = timer_read();
            common_layer_data.last_repeat_interval = 500;
            common_layer_data.last_repeat_record = *record;
        }
    } else if (common_layer_data.last_repeat_key &&
               (common_layer_data.last_repeat_key == key || key == KC_LGUI ||
                key == KC_RGUI || key == KC_LCTL || key == KC_RCTL ||
                key == KC_LSFT || key == KC_RSFT ||
                (key >= SAFE_RANGE && key < SAFE_RANGE + NUMBER_OF_LAYERS)))
        common_layer_data.last_repeat_key = 0;
    return true;
}

bool handle_common_key(uint16_t key, keyrecord_t *record) {
    switch (key) {
    case KEY_LEFT_COMMAND:
        if (record->event.pressed) {
            switch (common_layer_data.os) {
            case MACOS:
                register_code(KC_LGUI);
                break;
            case LINUX:
            case WINDOWS:
                register_code(KC_LCTL);
                break;
            }
        } else {
            switch (common_layer_data.os) {
            case MACOS:
                unregister_code(KC_LGUI);
                break;
            case LINUX:
            case WINDOWS:
                unregister_code(KC_LCTL);
                break;
            }
        }
        return false;
    case KEY_RIGHT_COMMAND:
        if (record->event.pressed) {
            switch (common_layer_data.os) {
            case MACOS:
                register_code(KC_RGUI);
                break;
            case LINUX:
            case WINDOWS:
                register_code(KC_RCTL);
                break;
            }
        } else {
            switch (common_layer_data.os) {
            case MACOS:
                unregister_code(KC_RGUI);
                break;
            case LINUX:
            case WINDOWS:
                unregister_code(KC_RCTL);
                break;
            }
        }
        return false;
    case KEY_LEFT_CONTROL:
        if (record->event.pressed) {
            switch (common_layer_data.os) {
            case MACOS:
                register_code(KC_LCTL);
                break;
            case LINUX:
            case WINDOWS:
                register_code(KC_LGUI);
                break;
            }
        } else {
            switch (common_layer_data.os) {
            case MACOS:
                unregister_code(KC_LCTL);
                break;
            case LINUX:
            case WINDOWS:
                unregister_code(KC_LGUI);
                break;
            }
        }
        return false;
    case KEY_RIGHT_CONTROL:
        if (record->event.pressed) {
            switch (common_layer_data.os) {
            case MACOS:
                register_code(KC_RCTL);
                break;
            case LINUX:
            case WINDOWS:
                register_code(KC_RGUI);
                break;
            }
        } else {
            switch (common_layer_data.os) {
            case MACOS:
                unregister_code(KC_RCTL);
                break;
            case LINUX:
            case WINDOWS:
                unregister_code(KC_RGUI);
                break;
            }
        }
        return false;
    case KEY_MOVE_PREVIOUS_WORD:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LALT(KC_LEFT));
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_MOVE_NEXT_WORD:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LALT(KC_RIGHT));
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_MOVE_LINE_STRAT:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                switch (common_layer_data.application) {
                case DEFAULT:
                    tap_code16(LGUI(KC_LEFT));
                    break;
                case TERMINAL:
                    tap_code16(LCTL(KC_A));
                    break;
                }
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_MOVE_LINE_END:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                switch (common_layer_data.application) {
                case DEFAULT:
                    tap_code16(LGUI(KC_RIGHT));
                    break;
                case TERMINAL:
                    tap_code16(LCTL(KC_E));
                    break;
                }
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_DELETE_WORD:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LSFT(LALT(KC_LEFT)));
                tap_code(KC_BSPC);
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_DELETE_LINE:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LGUI(KC_RIGHT));
                tap_code16(LSFT(LGUI(KC_LEFT)));
                tap_code(KC_BSPC);
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KC_ENT:
        if (get_mods() & MOD_MASK_SHIFT) {
            if (record->event.pressed)
                switch (common_layer_data.os) {
                case MACOS:
                    unregister_code(KC_LSFT);
                    unregister_code(KC_RSFT);
                    tap_code16(LGUI(KC_LEFT));
                    tap_code(KC_ENTER);
                    tap_code(KC_UP);
                    break;
                case LINUX:
                    break;
                case WINDOWS:
                    break;
                }
            return false;
        }
        return true;
    case KEY_CREATE_NEXT_LINE:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LGUI(KC_RIGHT));
                tap_code(KC_ENTER);
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_CREATE_CPP_STATEMENT:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LGUI(KC_RIGHT));
                tap_code(KC_SCLN);
                tap_code(KC_ENTER);
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_CREATE_CPP_BODY:
        if (record->event.pressed)
            switch (common_layer_data.os) {
            case MACOS:
                tap_code16(LGUI(KC_RIGHT));
                tap_code16(KC_LCBR);
                tap_code(KC_ENTER);
                break;
            case LINUX:
                break;
            case WINDOWS:
                break;
            }
        return false;
    case KEY_CREATE_LATEX_LEFT:
        if (record->event.pressed) {
            tap_code(KC_BSLS);
            tap_code(KC_L);
            tap_code(KC_E);
            tap_code(KC_F);
            tap_code(KC_T);
        }
        return false;
    case KEY_CREATE_LATEX_RIGHT:
        if (record->event.pressed) {
            tap_code(KC_BSLS);
            tap_code(KC_R);
            tap_code(KC_I);
            tap_code(KC_G);
            tap_code(KC_H);
            tap_code(KC_T);
        }
        return false;
    }
    if (key >= SAFE_RANGE + NUMBER_OF_LAYERS &&
        key < SAFE_RANGE + NUMBER_OF_LAYERS + NUMBER_OF_OSS) {
        common_layer_data.os = key - SAFE_RANGE - NUMBER_OF_LAYERS;
        return false;
    }
    return true;
}

bool process_record_user(uint16_t key, keyrecord_t *record) {
    if (!handle_layer_key(key, record))
        return false;
    if (!handle_repeat_key(key, record))
        return false;
    if (!handle_call_key(key, record))
        return false;
    if (!handle_common_key(key, record))
        return false;
    return true;
}

void matrix_scan_user() {
    if (common_layer_data.last_repeat_key &&
        timer_elapsed(common_layer_data.last_repeat_time) >
            common_layer_data.last_repeat_interval) {
        common_layer_data.last_repeat_time = timer_read();
        process_record(&common_layer_data.last_repeat_record);
        if (common_layer_data.last_repeat_interval == 500)
            common_layer_data.last_repeat_interval =
                common_layer_data.last_repeat_interval / 27 * 8;
        else if (common_layer_data.last_repeat_interval != 50) {
            common_layer_data.last_repeat_interval =
                common_layer_data.last_repeat_interval / 3 * 2;
            if (common_layer_data.last_repeat_interval < 50)
                common_layer_data.last_repeat_interval = 50;
        }
    }
}

#define KEYMAP_PERMUTE( \
    k00, k01, k02, k03,k04, k05, k06, \
    k10, k11, k12, k13, k14, k15, k16, \
    k20, k21, k22, k23, k24, k25, \
    k30, k31, k32, k33, k34, k35, k36, \
    k40, k41, k42, k43, k44, \
    k55, k56, \
    k54, \
    k53, k52, k51, \
    k07, k08, k09, k0A, k0B, k0C, k0D, \
    k17, k18, k19, k1A, k1B, k1C, k1D, \
    k28, k29, k2A, k2B, k2C, k2D, \
    k37, k38, k39, k3A, k3B, k3C, k3D, \
    k49, k4A, k4B, k4C, k4D, \
    k57, k58, \
    k59, \
    k5C, k5B, k5A\
) \
LAYOUT_moonlander( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0A, k0B, k0C, k0D, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1A, k1B, k1C, k1D, \
    k20, k21, k22, k23, k24, k25, k36, k37, k28, k29, k2A, k2B, k2C, k2D, \
    k30, k31, k32, k33, k34, k35, k38, k39, k3A, k3B, k3C, k3D, \
    k40, k41, k42, k43, k44, k54, k59, k49, k4A, k4B, k4C, k4D, \
    k53, k52, k51, k5C, k5B, k5A \
) \

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_NORM_BASE] = KEYMAP_PERMUTE(
        KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5, KEY_FORWARD_LAYER(LAYER_WINDOW),
        KC_ENT, KC_Q, KC_W, KC_F, KC_P, KC_G, KC_CAPS,
        KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KC_A, KC_R, KC_S, KC_T, KC_D,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, LCTL(KC_SPC),
        KEY_LEFT_COMMAND, KEY_LEFT_CONTROL, KC_LALT, KC_DEL, KC_BSPC,
        KC_NO, KC_NO,
        KEY_FORWARD_LAYER(LAYER_GAME_BASE),
        KC_LBRC, LCTL(KC_SPC), KEY_CREATE_LATEX_LEFT,
        KEY_FORWARD_LAYER(LAYER_UTILITY), KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO,
        KC_ESC, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_LCBR,
        KC_H, KC_N, KC_E, KC_I, KC_O, KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION),
        LOPT(KC_SPC), KC_K, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT,
        KC_SPC, KEY_CREATE_CPP_STATEMENT, KC_RALT, KEY_RIGHT_CONTROL, KEY_RIGHT_COMMAND,
        KC_NO, KC_NO,
        KEY_FORWARD_LAYER(LAYER_GAME_BASE),
        KEY_CREATE_LATEX_RIGHT, LOPT(KC_SPC), KC_LPRN
    ),
    [LAYER_NORM_EXTENSION] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_FORWARD_LAYER(LAYER_OS),
        KEY_CREATE_NEXT_LINE, KC_AT, KC_CIRC, KC_PIPE, KC_QUES, KC_PERC, KC_CAPS,
        KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION), KEY_MOVE_PREVIOUS_WORD, KEY_MOVE_NEXT_WORD, KC_AMPR, KEY_MOVE_LINE_STRAT, KEY_MOVE_LINE_END,
        KC_LSFT, KC_DLR, KC_HASH, KC_BSLS, KC_TILD, KC_UNDS, LCTL(KC_SPC),
        KEY_LEFT_COMMAND, KEY_LEFT_CONTROL, KC_LALT, KEY_DELETE_LINE, KEY_DELETE_WORD,
        KC_NO, KC_NO,
        KEY_FORWARD_LAYER(LAYER_GAME_BASE),
        KC_RBRC, LCTL(KC_SPC), KEY_CREATE_LATEX_LEFT,
        KEY_FORWARD_LAYER(LAYER_FUNCTION), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_EQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR,
        KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_GRAVE, KEY_FORWARD_LAYER(LAYER_NORM_EXTENSION),
        KC_NO, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT,
        KC_TAB, KEY_CREATE_CPP_BODY, KC_RALT, KEY_RIGHT_CONTROL, KEY_RIGHT_COMMAND,
        KC_NO, KC_NO,
        KEY_FORWARD_LAYER(LAYER_GAME_BASE),
        KEY_CREATE_LATEX_RIGHT, LOPT(KC_SPC), KC_RPRN
    ),
    [LAYER_GAME_BASE] = KEYMAP_PERMUTE(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KEY_FORWARD_LAYER(LAYER_WINDOW),
        KC_ENT, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y,
        KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION), KC_A, KC_S, KC_D, KC_F, KC_G,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_H,
        KEY_LEFT_CONTROL, KEY_LEFT_COMMAND, KC_LALT, KC_LALT, KC_SPC,
        KC_NO, KC_NO,
        KEY_BACK_LAYER,
        KC_LBRC, LCTL(KC_SPC), KEY_CREATE_LATEX_LEFT,
        KEY_FORWARD_LAYER(LAYER_UTILITY), KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO,
        KC_ESC, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LCBR,
        KC_H, KC_J, KC_K, KC_L, KC_SCLN, KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION),
        LOPT(KC_SPC), KC_N, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_RSFT,
        KC_BSPC, KEY_CREATE_CPP_STATEMENT, KC_RALT, KEY_RIGHT_COMMAND, KEY_RIGHT_CONTROL,
        KC_NO, KC_NO,
        KEY_BACK_LAYER,
        KEY_CREATE_LATEX_RIGHT, LOPT(KC_SPC), KC_LPRN
    ),
    [LAYER_GAME_EXTENSION] = KEYMAP_PERMUTE(
        KC_ESC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_FORWARD_LAYER(LAYER_OS),
        KEY_CREATE_NEXT_LINE, KC_AT, KC_CIRC, KC_PIPE, KC_QUES, KC_PERC, KC_CAPS,
        KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION), KEY_MOVE_PREVIOUS_WORD, KEY_MOVE_NEXT_WORD, KC_AMPR, KEY_MOVE_LINE_STRAT, KEY_MOVE_LINE_END,
        KC_LSFT, KC_DLR, KC_HASH, KC_BSLS, KC_TILD, KC_UNDS, LCTL(KC_SPC),
        KEY_LEFT_CONTROL, KEY_LEFT_COMMAND, KC_LALT, KEY_DELETE_LINE, KC_TAB,
        KC_NO, KC_NO,
        KEY_BACK_LAYER,
        KC_RBRC, LCTL(KC_SPC), KEY_CREATE_LATEX_LEFT,
        KEY_FORWARD_LAYER(LAYER_FUNCTION), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_EQL, KC_PLUS, KC_MINUS, KC_EXLM, KC_COLN, KC_RCBR,
        KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_GRAVE, KEY_FORWARD_LAYER(LAYER_GAME_EXTENSION),
        KC_NO, KC_ASTR, KC_SLSH, KC_LT, KC_GT, KC_DQUO, KC_RSFT,
        KEY_DELETE_WORD, KEY_CREATE_CPP_BODY, KC_RALT, KEY_RIGHT_COMMAND, KEY_RIGHT_CONTROL,
        KC_NO, KC_NO,
        KEY_BACK_LAYER,
        KEY_CREATE_LATEX_RIGHT, LOPT(KC_SPC), KC_RPRN
    ),
    [LAYER_OS] = KEYMAP_PERMUTE(
        KC_NO, KEY_SWITCH_OS(MACOS), KEY_SWITCH_OS(LINUX), KEY_SWITCH_OS(WINDOWS), KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
    [LAYER_FUNCTION] = KEYMAP_PERMUTE(
        KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F11,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_F12, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
    [LAYER_WINDOW] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KEY_BACK_LAYER,
        KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LEFT, KC_NO, KC_NO, KC_RIGHT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KEY_BACK_LAYER, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
    [LAYER_UTILITY] = KEYMAP_PERMUTE(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_MPLY, KC_NO, KC_NO,
        KC_NO, KC_VOLD, KC_VOLU, LGUI(LSFT(KC_4)), KEY_DANCE(DANCE_PGDN_BOTTOM), KEY_DANCE(DANCE_PGUP_TOP),
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO,
        KC_NO, KC_NO, KC_NO
    ),
};
