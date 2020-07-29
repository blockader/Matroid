#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "usb_descriptor.h"
#include <stdio.h>
#include <string.h>

enum {
    LAYER_NORM_BASE,
    LAYER_NORM_EXTENSION,
    LAYER_RACE_BASE,
    LAYER_RACE_EXTENSION,
    LAYER_GAME_BASE,
    LAYER_GAME_EXTENSION,
    LAYER_LEGACY_BASE,
    LAYER_LEGACY_EXTENSION,
    LAYER_CONTROL,
    LAYER_WINDOW,
    LAYER_DESKTOP,
    NUMBER_OF_LAYERS,
};

bool temporary[NUMBER_OF_LAYERS] = {
    [LAYER_NORM_BASE] = false,   [LAYER_NORM_EXTENSION] = true,
    [LAYER_RACE_BASE] = false,   [LAYER_RACE_EXTENSION] = true,
    [LAYER_GAME_BASE] = false,   [LAYER_GAME_EXTENSION] = true,
    [LAYER_LEGACY_BASE] = false, [LAYER_LEGACY_EXTENSION] = true,
    [LAYER_CONTROL] = false,     [LAYER_WINDOW] = true,
    [LAYER_DESKTOP] = false,
};

uint8_t layers[16];

enum OS {
    MACOS,
    LINUX,
    WINDOWS,
};

enum Application {
    DEFAULT,
    TERMINAL,
    NUMBER_OF_APPLICATIONS,
};

struct {
    bool ended, handness_enabled, backlight_enabled, slave;
    int8_t last_nonspace_handness, modifier_handness;
    uint8_t os, application;
    uint16_t last_repeat_key;
    int last_nonspace_time, last_repeat_time, last_repeat_interval;
} common_layer_data;

struct {
    int operator, multiplier;
} layer_control_data;

struct {
    int start_time;
} layer_window_data;

#define KEY_FORWARD_LAYER(a) SAFE_RANGE + a

enum {
    DANCE_PGDN_BOTTOM,
    DANCE_PGUP_TOP,
    DANCE_LPRN_RPRN,
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [DANCE_PGDN_BOTTOM] = ACTION_TAP_DANCE_DOUBLE(KC_PGDN, LGUI(KC_DOWN)),
    [DANCE_PGUP_TOP] = ACTION_TAP_DANCE_DOUBLE(KC_PGUP, LGUI(KC_UP)),
    [DANCE_LPRN_RPRN] = ACTION_TAP_DANCE_DOUBLE(KC_LPRN, KC_RPRN),
};

#define KEY_DANCE(a) TD(a)

#define TRAIT_LEFT(a) a
#define TRAIT_RIGHT(a) a

enum custom_keycodes {
    KEY_BACK_LAYER = SAFE_RANGE + NUMBER_OF_LAYERS,
    KEY_INSERT_LINE_START,
    KEY_INSERT_HERE,
    KEY_INSERT_LINE_END,
    KEY_CREATE_PREVIOUS_LINE,
    KEY_CREATE_NEXT_LINE,
    KEY_DELETE_WORD,
    KEY_DELETE_LINE,
    KEY_CUT_WORD,
    KEY_CUT_LINE,
    KEY_CUT_SELECTION,
    KEY_MOVE_LINE_STRAT,
};

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
    else if (!strcmp(m->command, "handness")) {
        int v;
        if (sscanf(m->arguments, "%d", &v) != 1 || v < -1 || v > 1) {
            send_confusion(m);
        } else {
            if (v == -1)
                sprintf(m->arguments, "%d",
                        (int)common_layer_data.handness_enabled);
            else if (common_layer_data.handness_enabled == v)
                m->arguments = "redundancy";
            else {
                common_layer_data.handness_enabled = v;
                m->arguments = "success";
            }
            send_message(m);
        }
    } else if (!strcmp(m->command, "backlight")) {
        int v;
        if (sscanf(m->arguments, "%d", &v) != 1 || v < -1 || v > 1) {
            send_confusion(m);
        } else {
            if (v == -1)
                sprintf(m->arguments, "%d",
                        (int)common_layer_data.backlight_enabled);
            else if (common_layer_data.backlight_enabled == v)
                m->arguments = "redundancy";
            else {
                common_layer_data.backlight_enabled = v;
#ifdef BACKLIGHT_ENABLED
                if (v)
                    rgb_matrix_enable();
                else
                    rgb_matrix_disable();
#endif
                m->arguments = "success";
            }
            send_message(m);
        }
    } else if (!strcmp(m->command, "time")) {
        sprintf(m->arguments, "%d", timer_read());
        send_message(m);
    } else if (!strcmp(m->command, "application")) {
        int v;
        if (sscanf(m->arguments, "%d", &v) != 1 || v < 0 ||
            v >= NUMBER_OF_APPLICATIONS) {
            send_confusion(m);
        } else {
            common_layer_data.application = v;
            m->arguments = "success";
            send_message(m);
        }
    } else if (!strcmp(m->command, "slave")) {
        int v;
        if (sscanf(m->arguments, "%d", &v) != 1 || v < 0 || v > 1) {
            send_confusion(m);
        } else {
            common_layer_data.slave = v;
            m->arguments = "success";
            send_message(m);
        }
    } else if (!strcmp(m->command, "key")) {
        int x, y, p;
        if (sscanf(m->arguments, "%d %d %d", &x, &y, &p) != 3 || x < 0 ||
            y < 0 || x >= MATRIX_ROWS || y >= MATRIX_COLS) {
            send_confusion(m);
        } else {
            keyrecord_t k;
            k.event.pressed = p;
            k.event.key.row = x;
            k.event.key.col = y;
            process_record(&k);
            m->arguments = "success";
            send_message(m);
        }
    } else {
        send_confusion(m);
    }
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

extern const int8_t handness[MATRIX_ROWS][MATRIX_COLS];

uint16_t translate_key(uint16_t key) {
    switch (key) {
    case KEY_MOVE_LINE_STRAT:
        switch (common_layer_data.os) {
        case MACOS:
            switch (common_layer_data.application) {
            case TERMINAL:
                return LCTL(KC_A);
            default:
                return LGUI(KC_LEFT);
            }
        case LINUX:
            return key;
        case WINDOWS:
            return key;
        }
    default:
        return key;
    }
}

bool handle_layer_key(uint16_t key, keyrecord_t *record) {
    switch (layers[layers[0] + 1]) {
    case LAYER_CONTROL:
        if (key >= KC_1 && key <= KC_0) {
            if (record->event.pressed) {
                layer_control_data.multiplier =
                    layer_control_data.multiplier * 10;
                if (key != KC_0)
                    layer_control_data.multiplier += key - KC_1 + 1;
                if (layer_control_data.multiplier < 0)
                    layer_control_data.multiplier = 0;
            }
            return false;
        }
        switch (key) {
        case KEY_INSERT_LINE_START:
            if (record->event.pressed)
                common_layer_data.ended = true;
            return true;
        case KEY_INSERT_HERE:
            if (record->event.pressed)
                common_layer_data.ended = true;
            return true;
        case KEY_INSERT_LINE_END:
            if (record->event.pressed)
                common_layer_data.ended = true;
            return true;
        case KEY_CREATE_PREVIOUS_LINE:
            if (record->event.pressed)
                common_layer_data.ended = true;
            return true;
        case KEY_CUT_WORD:
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1) {
                    register_code(KC_LALT);
                    tap_code(KC_LEFT);
                    tap_code(KC_RIGHT);
                    unregister_code(KC_LALT);
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(LALT(KC_LEFT)));
                    tap_code16(LGUI(KC_X));
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case LALT(KC_LEFT):
            if (layer_control_data.operator== - 1 &&
                layer_control_data.multiplier == 0)
                return true;
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LALT(KC_LEFT));
                else if (layer_control_data.operator== KC_BSPC) {
                    register_code(KC_LALT);
                    tap_code(KC_LEFT);
                    tap_code(KC_RIGHT);
                    unregister_code(KC_LALT);
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(LALT(KC_LEFT)));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    register_code(KC_LALT);
                    tap_code(KC_LEFT);
                    tap_code(KC_RIGHT);
                    unregister_code(KC_LALT);
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(LALT(KC_LEFT)));
                    tap_code16(LGUI(KC_C));
                    tap_code(KC_RIGHT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case LALT(KC_RIGHT):
            if (layer_control_data.operator== - 1 &&
                layer_control_data.multiplier == 0)
                return true;
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LALT(KC_RIGHT));
                else if (layer_control_data.operator== KC_BSPC) {
                    register_code(KC_LALT);
                    tap_code(KC_RIGHT);
                    tap_code(KC_LEFT);
                    unregister_code(KC_LALT);
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(LALT(KC_RIGHT)));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    register_code(KC_LALT);
                    tap_code(KC_RIGHT);
                    tap_code(KC_LEFT);
                    unregister_code(KC_LALT);
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(LALT(KC_RIGHT)));
                    tap_code16(LGUI(KC_C));
                    tap_code(KC_LEFT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case KEY_MOVE_LINE_STRAT:
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    tap_code16(translate_key(KEY_MOVE_LINE_STRAT));
                else if (layer_control_data.operator== KC_BSPC) {
                    tap_code16(LSFT(translate_key(KEY_MOVE_LINE_STRAT)));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    tap_code16(LSFT(translate_key(KEY_MOVE_LINE_STRAT)));
                    tap_code16(LGUI(KC_C));
                    tap_code(KC_RIGHT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case LCTL(KC_E):
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    tap_code16(LCTL(KC_E));
                else if (layer_control_data.operator== KC_BSPC) {
                    tap_code16(LSFT(LCTL(KC_E)));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    tap_code16(LSFT(LCTL(KC_E)));
                    tap_code16(LGUI(KC_C));
                    tap_code(KC_LEFT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case KC_BSPC:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    unregister_code(KC_LSHIFT);
                    tap_code16(LGUI(KC_X));
                    register_code(KC_LSHIFT);
                    layer_control_data.multiplier = 0;
                    layer_control_data.operator= - 1;
                } else if (layer_control_data.operator== - 1)
                    layer_control_data.operator= KC_BSPC;
                else {
                    if (layer_control_data.operator== KC_BSPC) {
                        tap_code16(LCTL(KC_A));
                        for (int i = 0; i < (layer_control_data.multiplier
                                                 ? layer_control_data.multiplier
                                                 : 1);
                             ++i)
                            tap_code16(LSFT(KC_DOWN));
                        tap_code16(LGUI(KC_X));
                    }
                    layer_control_data.multiplier = 0;
                    layer_control_data.operator= - 1;
                }
            }
            return false;
        case KC_LEFT:
        case KC_RIGHT:
            if (layer_control_data.operator== - 1 &&
                layer_control_data.multiplier == 0)
                return true;
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code(key);
                else if (layer_control_data.operator== KC_BSPC) {
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(key));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(key));
                    tap_code16(LGUI(KC_C));
                    tap_code(key == KC_RIGHT ? KC_LEFT : KC_RIGHT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case KC_DOWN:
            if (layer_control_data.operator== - 1 &&
                layer_control_data.multiplier == 0)
                return true;
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code(KC_DOWN);
                else if (layer_control_data.operator== KC_BSPC) {
                    tap_code16(LCTL(KC_A));
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(KC_DOWN));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    tap_code16(LCTL(KC_A));
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(KC_DOWN));
                    tap_code16(LGUI(KC_C));
                    tap_code(KC_LEFT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case KC_UP:
            if (layer_control_data.operator== - 1 &&
                layer_control_data.multiplier == 0)
                return true;
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code(KC_UP);
                else if (layer_control_data.operator== KC_BSPC) {
                    tap_code16(LCTL(KC_E));
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(KC_UP));
                    tap_code16(LSFT(LCTL(KC_E)));
                    tap_code16(LGUI(KC_X));
                } else if (layer_control_data.operator== LGUI(KC_C)) {
                    tap_code16(LCTL(KC_E));
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LSFT(KC_UP));
                    tap_code16(LSFT(LCTL(KC_E)));
                    tap_code16(LGUI(KC_C));
                    tap_code(KC_RIGHT);
                }
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case KEY_CREATE_NEXT_LINE:
            if (record->event.pressed)
                common_layer_data.ended = true;
            return true;
        case LGUI(KC_Z):
            if (layer_control_data.operator== - 1 &&
                layer_control_data.multiplier == 0)
                return true;
            if (record->event.pressed) {
                if (layer_control_data.operator== - 1)
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LGUI(KC_Z));
                layer_control_data.multiplier = 0;
                layer_control_data.operator= - 1;
            }
            return false;
        case LGUI(KC_C):
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    unregister_code(KC_LSHIFT);
                    tap_code16(LGUI(KC_C));
                    register_code(KC_LSHIFT);
                    layer_control_data.multiplier = 0;
                    layer_control_data.operator= - 1;
                } else if (layer_control_data.operator== - 1)
                    layer_control_data.operator= LGUI(KC_C);
                else {
                    if (layer_control_data.operator== LGUI(KC_C)) {
                        tap_code16(LCTL(KC_A));
                        for (int i = 0; i < (layer_control_data.multiplier
                                                 ? layer_control_data.multiplier
                                                 : 1);
                             ++i)
                            tap_code16(LSFT(KC_DOWN));
                        tap_code16(LGUI(KC_C));
                        tap_code(KC_LEFT);
                    }
                    layer_control_data.multiplier = 0;
                    layer_control_data.operator= - 1;
                }
            }
            return false;
        case LGUI(KC_V):
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    unregister_code(KC_LSHIFT);
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LGUI(KC_V));
                    register_code(KC_LSHIFT);
                    layer_control_data.multiplier = 0;
                    layer_control_data.operator= - 1;
                } else {
                    for (int i = 0; i < (layer_control_data.multiplier
                                             ? layer_control_data.multiplier
                                             : 1);
                         ++i)
                        tap_code16(LGUI(KC_V));
                    layer_control_data.multiplier = 0;
                    layer_control_data.operator= - 1;
                }
            }
            return false;
        }
        layer_control_data.multiplier = 0;
        layer_control_data.operator= - 1;
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
    case LAYER_DESKTOP:
        switch (key) {
        case KEY_BACK_LAYER:
            if (record->event.pressed)
                tap_code16(LCTL(KC_UP));
            return true;
        }
        return true;
    }
    return true;
}

void handle_layer_start(uint16_t key, keyrecord_t *record) {
    rgblight_disable_noeeprom();
    struct message m;
    switch (layers[layers[0] + 1]) {
    case LAYER_RACE_BASE:
        m.session_id = timer_read();
        m.command = "language";
        m.arguments = "english";
        send_message(&m);
    case LAYER_RACE_EXTENSION:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_BLUE);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        return;
    case LAYER_LEGACY_BASE:
    case LAYER_LEGACY_EXTENSION:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_GREEN);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        return;
    case LAYER_CONTROL:
        layer_control_data.operator= - 1;
        layer_control_data.multiplier = 0;
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        return;
    case LAYER_WINDOW:
        register_code(KC_LGUI);
        tap_code(KC_TAB);
        layer_window_data.start_time = timer_read();
        return;
    case LAYER_DESKTOP:
        tap_code16(LCTL(KC_UP));
        return;
    }
}

void handle_layer_return(void) {
    rgblight_disable_noeeprom();
    switch (layers[layers[0] + 1]) {
    case LAYER_RACE_BASE:
    case LAYER_RACE_EXTENSION:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_BLUE);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        return;
    case LAYER_LEGACY_BASE:
    case LAYER_LEGACY_EXTENSION:
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_GREEN);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        return;
    case LAYER_CONTROL:
        layer_control_data.operator= - 1;
        layer_control_data.multiplier = 0;
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
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
    common_layer_data.ended = false;
    layer_on(layers[layers[0] + 1]);
}

bool handle_call_key(uint16_t key, keyrecord_t *record) {
    switch (key) {
    case KEY_BACK_LAYER:
        if (record->event.pressed) {
            if (layers[0]) {
                handle_layer_end();
                --layers[0];
                update_layer();
                handle_layer_return();
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
                handle_layer_start(key, record);
            }
        } else {
            if (layers[layers[0] + 1] == new_layer &&
                temporary[layers[layers[0] + 1]]) {
                handle_layer_end();
                --layers[0];
                update_layer();
                handle_layer_return();
            }
        }
        return false;
    }
    if (common_layer_data.ended) {
        if (layers[0]) {
            handle_layer_end();
            --layers[0];
            update_layer();
            handle_layer_return();
        }
    }
    return true;
}

uint8_t unregister_shift(void) {
    uint8_t s = 0;
    if (get_mods() & MOD_BIT(KC_LSFT)) {
        unregister_code(KC_LSFT);
        s = 1;
    }
    if (get_mods() & MOD_BIT(KC_RSFT)) {
        unregister_code(KC_RSFT);
        s += 2;
    }
    return s;
}

void register_shift(uint8_t s) {
    if (s & 1)
        register_code(KC_LSFT);
    if (s & 2)
        register_code(KC_RSFT);
}

bool handle_common_key(uint16_t key, keyrecord_t *record) {
    switch (key) { // TODO: This is an awful way to have some shifted keys work the same as they are in the extension layer. It's better to look up at the keymaps array.
    case KC_HASH:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                handle_call_key(SAFE_RANGE + LAYER_WINDOW, record);
                register_shift(s);
            } else
                tap_code16(KC_HASH);
        }
        return false;
    case KC_1:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                tap_code16(LOPT(KC_SPC));
                register_shift(s);
            } else
                tap_code(KC_1);
        }
        return false;
    case KC_2:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                tap_code16(LCTL(KC_SPC));
                register_shift(s);
            } else
                tap_code(KC_2);
        }
        return false;
    case KC_3:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                tap_code(KC_GRAVE);
                register_shift(s);
            } else
                tap_code(KC_3);
        }
        return false;
    case KC_4:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s)
                ;
            else
                tap_code(KC_4);
        }
        return false;
    case KC_5:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s)
                ;
            else
                tap_code(KC_5);
        }
        return false;
    case KC_6:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s)
                ;
            else
                tap_code(KC_6);
        }
        return false;

    case KC_7:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s)
                ;
            else
                tap_code(KC_7);
        }
        return false;
    case KC_8:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                tap_code(KC_ESC);
                register_shift(s);
            } else
                tap_code(KC_8);
        }
        return false;
    case KC_9:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                tap_code16(LGUI(LSFT(KC_4)));
                register_shift(s);
            } else
                tap_code(KC_9);
        }
        return false;
    case KC_0:
        if (record->event.pressed) {
            uint8_t s = unregister_shift();
            if (s) {
                tap_code(KC_MPLY);
                register_shift(s);
            } else
                tap_code(KC_0);
        }
        return false;
    case KEY_INSERT_LINE_START:
        if (record->event.pressed)
            tap_code16(LGUI(KC_LEFT));
        return false;
    case KEY_INSERT_HERE:
        return false;
    case KEY_INSERT_LINE_END:
        if (record->event.pressed)
            tap_code16(LGUI(KC_RIGHT));
        return false;
    case KEY_CREATE_PREVIOUS_LINE:
        if (record->event.pressed) {
            tap_code16(LGUI(KC_LEFT));
            tap_code(KC_ENTER);
            tap_code(KC_UP);
        }
        return false;
    case KEY_CREATE_NEXT_LINE:
        if (record->event.pressed) {
            tap_code16(LGUI(KC_RIGHT));
            tap_code(KC_ENTER);
        }
        return false;
    case KEY_DELETE_WORD:
        if (record->event.pressed) {
            register_code(KC_LALT);
            tap_code(KC_LEFT);
            tap_code(KC_RIGHT);
            unregister_code(KC_LALT);
            tap_code16(LSFT(LALT(KC_LEFT)));
            tap_code(KC_BSPC);
        }
        return false;
    case KEY_DELETE_LINE:
        if (record->event.pressed) {
            tap_code16(LGUI(KC_RIGHT));
            tap_code16(LSFT(LGUI(KC_LEFT)));
            tap_code(KC_BSPC);
        }
        return false;
    case KEY_CUT_WORD:
        if (record->event.pressed) {
            register_code(KC_LALT);
            tap_code(KC_LEFT);
            tap_code(KC_RIGHT);
            unregister_code(KC_LALT);
            tap_code16(LSFT(LALT(KC_LEFT)));
            tap_code16(LGUI(KC_X));
        }
        return false;
    case KEY_CUT_LINE:
        if (record->event.pressed) {
            tap_code16(LGUI(KC_RIGHT));
            tap_code16(LSFT(LGUI(KC_LEFT)));
            tap_code16(LGUI(KC_X));
        }
        return false;
    case KEY_CUT_SELECTION:
        if (record->event.pressed) {
            if (get_mods() & MOD_MASK_SHIFT) {
                unregister_code(KC_LSHIFT);
                tap_code16(LGUI(KC_X));
                register_code(KC_LSHIFT);
            } else
                tap_code16(LGUI(KC_X));
        }
        return false;
    case KEY_MOVE_LINE_STRAT:
        if (record->event.pressed)
            tap_code16(translate_key(KEY_MOVE_LINE_STRAT));
        return false;
    }
    return true;
}

bool modifier(uint16_t key) {
    switch (key) {
    case KC_LSFT:
    case KC_RSFT:
    case KC_LCTL:
    case KC_RCTL:
    case KC_LGUI:
    case KC_RGUI:
    case KC_LOPT:
    case KC_ROPT:
        return true;
    }
    return false;
}

bool handle_handness_start(uint16_t key, keyrecord_t *record) {
    if (!common_layer_data.handness_enabled)
        return true;
    if (key >= SAFE_RANGE && key < SAFE_RANGE + NUMBER_OF_LAYERS &&
        temporary[key - SAFE_RANGE]) {
        if (record->event.pressed)
            common_layer_data.modifier_handness =
                handness[record->event.key.row][record->event.key.col];
        else
            common_layer_data.modifier_handness = 0;
        common_layer_data.modifier_handness = 0; // TODO: There should be a config variable instead of manually turning it off.
    } else if (!modifier(key)) {
        if (record->event.pressed) {
            if (handness[record->event.key.row][record->event.key.col] *
                    common_layer_data.modifier_handness >
                0) {
                bool ignore = key == KC_BSPC;
                if (get_mods() & MOD_MASK_GUI) {
                    switch (key) {
                    case KC_A:
                    case KC_Z:
                    case KC_X:
                    case KC_C:
                    case KC_V:
                        ignore = true;
                    }
                }
                if (get_mods() & MOD_MASK_CTRL)
                    ignore = true;
                if (!ignore)
                    return false;
            }
        }
    }
    if (key != KC_SPC)
        if (record->event.pressed) {
            common_layer_data.last_nonspace_time = timer_read();
            common_layer_data.last_nonspace_handness =
                handness[record->event.key.row][record->event.key.col];
        }
    return true;
}

bool handle_handness_end(uint16_t key, keyrecord_t *record) {
    if (!common_layer_data.handness_enabled)
        return true;
    switch (key) {
    case KC_SPC:
        if (record->event.pressed) {
            if (handness[record->event.key.row][record->event.key.col] *
                        common_layer_data.last_nonspace_handness >
                    0 &&
                timer_elapsed(common_layer_data.last_nonspace_time) < 1000)
                return false;
            else
                common_layer_data.last_nonspace_handness =
                    -handness[record->event.key.row][record->event.key.col];
        }
        return true;
    }
    if (modifier(key)) {
        if (record->event.pressed) {
            common_layer_data.modifier_handness =
                handness[record->event.key.row][record->event.key.col];
        } else {
            common_layer_data.modifier_handness = 0;
        }
        common_layer_data.modifier_handness = 0; // TODO: There should be a config variable instead of manually turning it off.
        return true;
    }
    return true;
}

bool handle_repeat_key(uint16_t key, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (key) {
        case KC_BSPC:
        case KC_UP:
        case KC_DOWN:
        case KC_LEFT:
        case KC_RIGHT:
        case LALT(KC_LEFT):
        case LALT(KC_RIGHT):
        case LGUI(KC_Z):
        case LGUI(KC_V):
            common_layer_data.last_repeat_key = key;
            common_layer_data.last_repeat_time = timer_read();
            common_layer_data.last_repeat_interval = 500;
            break;
        case KC_Z:
        case KC_V:
            if (get_mods() & MOD_MASK_GUI) {
                common_layer_data.last_repeat_key = key;
                common_layer_data.last_repeat_time = timer_read();
                common_layer_data.last_repeat_interval = 500;
            } else
                common_layer_data.last_repeat_key = 0;
            break;
        default:
            common_layer_data.last_repeat_key = 0;
        }
    } else if (common_layer_data.last_repeat_key == key)
        common_layer_data.last_repeat_key = 0;
    else if ((common_layer_data.last_repeat_key == KC_Z ||
              common_layer_data.last_repeat_key == KC_V) &&
             (key == KC_LGUI || key == KC_RGUI))
        common_layer_data.last_repeat_key = 0;
    return true;
}

bool process_record_user(uint16_t key, keyrecord_t *record) {
    if (common_layer_data.slave) {
        memset(send_message_buffer, 0, RAW_EPSIZE + 1);
        sprintf(send_message_buffer, "%d key %d %d %d", timer_read(),
                record->event.key.row, record->event.key.col,
                (int)record->event.pressed);
        raw_hid_send((uint8_t *)send_message_buffer, RAW_EPSIZE);
        return false;
    }
    if (!handle_handness_start(key, record))
        return false;
    if (!handle_layer_key(key, record))
        return false;
    if (!handle_call_key(key, record))
        return false;
    if (!handle_common_key(key, record))
        return false;
    if (!handle_handness_end(key, record))
        return false;
    if (!handle_repeat_key(key, record))
        return false;
    return true;
}

void keyboard_post_init_user() {
    rgblight_disable_noeeprom();
#ifdef BACKLIGHT_ENABLED
    rgb_matrix_disable();
#endif
    common_layer_data.handness_enabled = true;
}

void matrix_scan_user() {
    if (common_layer_data.last_repeat_key &&
        timer_elapsed(common_layer_data.last_repeat_time) >
            common_layer_data.last_repeat_interval) {
        common_layer_data.last_repeat_time = timer_read();
        tap_code16(common_layer_data.last_repeat_key);
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