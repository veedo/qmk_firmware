
 /* Copyright 2021 Dane Evans
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
  // SOFLE RGB
//#include <stdio.h>

#include <stdint.h>
#include "color.h"
#include "keyboard.h"
#include "keycodes.h"
#include "oled_driver.h"
#include "rgblight.h"
#include QMK_KEYBOARD_H
#include "info_config.h"
#include "quantum_keycodes.h"
#include "progmem.h"

#define INDICATOR_BRIGHTNESS 30

#define HSV_OVERRIDE_HELP(h, s, v, Override) h, s , Override
#define HSV_OVERRIDE(hsv, Override) HSV_OVERRIDE_HELP(hsv,Override)

enum {
    LEDPOS_INDICATOR = 0,
    LEDPOS_BACK,
    LEDPOS_BACK_TOPLEFT = LEDPOS_BACK,
    LEDPOS_BACK_TOPMID,
    LEDPOS_BACK_TOPRIGHT,
    LEDPOS_BACK_BOTRIGHT,
    LEDPOS_BACK_BOTMID,
    LEDPOS_BACK_BOTLEFT,
    LEDPOS_THUMB1 = 7,
    LEDPOS_SUPER = LEDPOS_THUMB1,
    LEDPOS_LCOL1 = 8,
    LEDPOS_LEFTSHIFT = LEDPOS_LCOL1,
    LEDPOS_TAB,
    LEDPOS_ESC,
    LEDPOS_TILDE,
    LEDPOS_LCOL2 = 12,
    LEDPOS_1 = LEDPOS_LCOL2,
    LEDPOS_THUMB2 = 16,
    LEDPOS_THUMB3 = 17,
    LEDPOS_LCOL3 = 18,
    LEDPOS_LCOL4 = 22,
    LEDPOS_THUMB4 = 26,
    LEDPOS_FN = LEDPOS_THUMB4,
    LEDPOS_THUMB5 = 27,
    LEDPOS_SPACE = LEDPOS_THUMB5,
    LEDPOS_LCOL5 = 28,
    LEDPOS_LCOL6 = 32,
    LEDPOS_RIGHT_OFFSET = 36,
    LEDPOS_ENTER = LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB5,
    LEDPOS_RCOL1 = LEDPOS_RIGHT_OFFSET+LEDPOS_LCOL1,
    LEDPOS_RCOL2 = LEDPOS_RIGHT_OFFSET+LEDPOS_LCOL2,
    LEDPOS_RCOL3 = LEDPOS_RIGHT_OFFSET+LEDPOS_LCOL3,
    LEDPOS_RCOL4 = LEDPOS_RIGHT_OFFSET+LEDPOS_LCOL4,
    LEDPOS_RCOL5 = LEDPOS_RIGHT_OFFSET+LEDPOS_LCOL5,
    LEDPOS_RCOL6 = LEDPOS_RIGHT_OFFSET+LEDPOS_LCOL6,
};

const uint8_t LEDPOS[128] = {
    /*[KC_LGUI]=LEDPOS_THUMB1,[KC_LALT]=LEDPOS_THUMB2,[KC_LCTL]=LEDPOS_THUMB3,[MO(1)]=LEDPOS_THUMB4,*/ [KC_SPC]=LEDPOS_THUMB5,
    /*[KC_LSFT]=LEDPOS_LEFTSHIFT,*/ [KC_TAB]=LEDPOS_TAB, [KC_ESCAPE]=LEDPOS_ESC, [KC_GRV]=LEDPOS_TILDE,
    [KC_1]=LEDPOS_LCOL2+0,[KC_Q]=LEDPOS_LCOL2+1,[KC_A]=LEDPOS_LCOL2+2,[KC_Z]=LEDPOS_LCOL2+3,
    [KC_2]=LEDPOS_LCOL3+3,[KC_W]=LEDPOS_LCOL3+2,[KC_S]=LEDPOS_LCOL3+1,[KC_X]=LEDPOS_LCOL3+0,
    [KC_3]=LEDPOS_LCOL4+0,[KC_E]=LEDPOS_LCOL4+1,[KC_D]=LEDPOS_LCOL4+2,[KC_C]=LEDPOS_LCOL4+3,
    [KC_4]=LEDPOS_LCOL5+3,[KC_R]=LEDPOS_LCOL5+2,[KC_F]=LEDPOS_LCOL5+1,[KC_V]=LEDPOS_LCOL5+0,
    [KC_5]=LEDPOS_LCOL6+0,[KC_T]=LEDPOS_LCOL6+1,[KC_G]=LEDPOS_LCOL6+2,[KC_B]=LEDPOS_LCOL6+3,

    /*[KC_RCTL]=LEDPOS_THUMB1,[KC_RALT]=LEDPOS_THUMB2,[KC_RSFT]=LEDPOS_THUMB3,[MO(2)]=LEDPOS_THUMB4,*/[KC_ENT]=LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB5,
    [KC_MINS]=LEDPOS_RCOL3+3,[KC_BSPC]=LEDPOS_RCOL3+2,[KC_QUOT]=LEDPOS_RCOL3+1,[KC_DEL]=LEDPOS_RCOL3+0,
    [KC_0]=LEDPOS_RCOL2+0,[KC_P]=LEDPOS_RCOL2+1,[KC_SCLN]=LEDPOS_RCOL2+2,[KC_SLSH]=LEDPOS_RCOL2+3,
    [KC_9]=LEDPOS_RCOL3+3,[KC_O]=LEDPOS_RCOL3+2,[KC_L]=LEDPOS_RCOL3+1,[KC_DOT]=LEDPOS_RCOL3+0,
    [KC_8]=LEDPOS_RCOL4+0,[KC_I]=LEDPOS_RCOL4+1,[KC_K]=LEDPOS_RCOL4+2,[KC_COMM]=LEDPOS_RCOL4+3,
    [KC_7]=LEDPOS_RCOL5+3,[KC_U]=LEDPOS_RCOL5+2,[KC_J]=LEDPOS_RCOL5+1,[KC_M]=LEDPOS_RCOL5+0,
    [KC_6]=LEDPOS_RCOL6+0,[KC_Y]=LEDPOS_RCOL6+1,[KC_H]=LEDPOS_RCOL6+2,[KC_N]=LEDPOS_RCOL6+3,
};

// Light combinations
#define SET_INDICATORS(hsv) \
    {LEDPOS_INDICATOR,1,hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_INDICATOR,1,hsv}
#define SET_UNDERGLOW(hsv) \
    {LEDPOS_BACK, 6, hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_BACK, 6, hsv}
#define SET_BACKLIGHT(hsv) \
    {LEDPOS_BACK, 6, hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_BACK, 6, hsv}
#define SET_NUMPAD(hsv)     \
    {35+15, 5, hsv},\
    {35+22, 3, hsv},\
    {35+27, 3, hsv}
#define SET_NUMROW(hsv) \
    {10, 2, hsv}, \
    {20, 2, hsv}, \
    {30, 2, hsv}, \
    {35+ 10, 2, hsv}, \
    {35+ 20, 2, hsv}, \
    {35+ 30, 2, hsv}
#define SET_THUMBS(hsv) \
    {LEDPOS_THUMB1,1,hsv}, \
    {LEDPOS_THUMB2,1,hsv}, \
    {LEDPOS_THUMB3,1,hsv}, \
    {LEDPOS_THUMB4,1,hsv}, \
    {LEDPOS_THUMB5,1,hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB1,1,hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB2,1,hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB3,1,hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB4,1,hsv}, \
    {LEDPOS_RIGHT_OFFSET+LEDPOS_THUMB5,1,hsv}

enum sofle_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_ARROWS,
    LAYER_MATH,
    LAYER_PROG,
};

enum custom_keycodes {
    KC_EPIPE = SAFE_RANGE,
    KC_VIMQ,
    KC_VIMW,
    KC_VIME,
    KC_VIMS,
    KC_VIMV,
    KC_VCPY,
    KC_LARR,
    KC_RARR,
    KC_LBIN,
    KC_RBIN,
    KC_LAEQ,
    KC_RAEQ,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY 0
 * ,-----------------------------------.                  ,-----------------------------------.
 * |  `  |  1  |  2  |  3  |  4  |  5  |                  |  6  |  7  |  8  |  9  |  0  |  -_ |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * | ESC |  Q  |  W  |  E  |  R  |  T  |                  |  Y  |  U  |  I  |  O  |  P  | Bspc|
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * | Tab |  A  |  S  |  D  |  F  |  G  |-------.  ,-------|  H  |  J  |  K  |  L  | ;:  |  '" |
 * |-----+-----+-----+-----+-----+-----|  PLAY |  | MUTE  |-----+-----+-----+-----+-----+-----|
 * |LShft|  Z  |  X  |  C  |  V  |  B  |-------|  |-------|  N  |  M  | <,  |  .> | /?  | Del |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          | LOS | LAlt| LCtl|LOWER| / Space /     \Enter \ |ARROW|RShft| RAlt| RCtl|
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */

[LAYER_BASE] = LAYOUT(
KC_GRV,  KC_1, KC_2, KC_3, KC_4, KC_5,                   KC_6, KC_7, KC_8,  KC_9,  KC_0,   KC_MINS,
KC_ESC,  KC_Q, KC_W, KC_E, KC_R, KC_T,                   KC_Y, KC_U, KC_I,  KC_O,  KC_P,   KC_BSPC,
KC_TAB,  KC_A, KC_S, KC_D, KC_F, KC_G,                   KC_H, KC_J, KC_K,  KC_L,  KC_SCLN,KC_QUOT,
KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_MPLY, KC_MUTE, KC_N, KC_M, KC_COMM,KC_DOT,KC_SLSH,KC_DEL,
         KC_LGUI,KC_LALT,KC_LCTL,MO(1),KC_SPC,  KC_ENT, MO(2),KC_RSFT,KC_RALT,KC_RCTL),
/* LOWER 1
 * ,-----------------------------------.                  ,-----------------------------------.
 * |PRNTS| F1  | F2  | F3  | F4  | F5  |                  | F6  | F7  | F8  | F9  | F10 | F11 |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |  !  |  @  |  #  |  $  |  %  |                  |  ^  |  &  |  *  |  +  |  ▽  |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |  ~  |  |  |  (  |  )  |  _  |-------.  ,-------|  ▶  |  [{ | }]  | - _ | = + | F12 |
 * |-----+-----+-----+-----+-----+-----| Reset |  |   ▽   |-----+-----+-----+-----+-----+-----|
 * |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |-------|  |-------|  ▽  |  ▽  | <-  |  -> |  \  |  ▽  |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          |  ▽  |  ▽  |  ▽  |  ▽  | /   ▽   /     \  ▽   \ |  ▽  |  ▽  |  ▽  |  ▽  |
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */
[LAYER_LOWER] = LAYOUT(
KC_PSCR,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,                      KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,
_______,KC_EXLM,KC_AT,KC_HASH,KC_DLR,KC_PERC,           KC_CIRC,KC_AMPR,KC_ASTR,KC_PPLS,_______, _______,
_______,KC_TILD,KC_PIPE,KC_LPRN,KC_RPRN,KC_UNDS,      KC_EPIPE,KC_LBRC,KC_RBRC,KC_MINS,KC_EQL,KC_F12,
_______,_______,_______,_______,_______,_______,QK_BOOT,  KC_MNXT,_______,_______,KC_LARR,KC_RARR,KC_BSLS,_______,
             _______,_______,_______,_______,_______,  _______,_______,_______,_______,_______),
/* ARROWS 2
 * ,-----------------------------------.                  ,-----------------------------------.
 * |PRNTS|  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |                  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  | :q  | :w  | :e  |MENU |PRNTS|                  | PGU |Home |  ⬆  | End | Ins |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |     | :s/ |  {  |  }  |  %  |-------.  ,-------| PGD |  ←  |  ↓  |  →  |     |  ▽  |
 * |-----+-----+-----+-----+--=--+-----|  PREV |  | NEXT  |-----+--=--+-----+-----+-----+-----|
 * |  ▽  |     |     |VCOPY| CR" |     |-------|  |-------|     | <=  | <<  |  >> |  => |  ▽  |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          |  ▽  |  ▽  |  ▽  |  ▽  | /   ▽   /     \  ▽   \ |  ▽  |  ▽  |  ▽  |  ▽  |
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */
[LAYER_ARROWS] = LAYOUT(
KC_PSCR,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,                          KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,
_______,KC_VIMQ,KC_VIMW,KC_VIME,KC_APP,KC_PSCR,                 KC_PGUP,KC_HOME,KC_UP,KC_END,KC_INS,_______,
_______,KC_NO,KC_VIMS,KC_LCBR,KC_RCBR,KC_PERC,            KC_PGDN,KC_LEFT,KC_DOWN,KC_RGHT,KC_NO,KC_F12,
_______,_______,_______,KC_VCPY,KC_VIMV,KC_NO,KC_MPRV,   KC_MNXT,KC_NO,KC_LAEQ,KC_LBIN,KC_RBIN,KC_RAEQ,_______,
             _______,_______,_______,_______,_______,   _______,_______,_______,_______,_______),
/* MATH 3
 * ,-----------------------------------.                  ,-----------------------------------.
 * |     |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |                  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |  [  |  ]  |  (  |  )  |  &  |                  |     |  7  |  8  |  9  |     |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |  /  |  *  |  -  |  +  |  =  |-------.  ,-------|  .  |  4  |  5  |  6  |  0  |  ▽  |
 * |-----+-----+-----+-----+-----+-----|   ▽   |  |   ▽   |-----+-----+-----+-----+-----+-----|
 * |MO(4)|  %  |  ^  |  {  |  }  |  |  |-------|  |-------|     |  1  |  2  |  3  |  .  |  ▽  |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          |  ▽  |  ▽  |  ▽  |  ▽  | /   ▽   /     \  ▽   \ |  ▽  |  ▽  |  ▽  |  ▽  |
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */
[LAYER_MATH] = LAYOUT(
KC_NO,_______,_______,_______,_______,_______,                  _______,_______,_______,_______,_______,_______,
_______,KC_LBRC,KC_RBRC,KC_LPRN,KC_RPRN,KC_AMPR,                KC_NO,KC_P7,KC_P8,KC_P9,KC_NO,_______,
_______,KC_PSLS,KC_PAST,KC_PMNS,KC_PPLS,KC_PEQL,              KC_PDOT,KC_P4,KC_P5,KC_P6,KC_P0,_______,
MO(4),KC_PERC,KC_CIRC,KC_LCBR,KC_RCBR,KC_PIPE,_______,  _______,KC_NO,KC_P1,KC_P2,KC_P3,KC_PDOT,_______,
           _______,_______,_______,_______,_______,     _______,_______,_______,_______,_______),
/* PROG 4
 * ,-----------------------------------.                  ,-----------------------------------.
 * |Reset|     |     |     |     |     |                  |     |     |     |     |     |     |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |Debug|     |     |     |     |     |                  |     |     |     |     |     |     |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |     |     |     |     |     |     |-------.  ,-------|     |     |     |     |     |     |
 * |-----+-----+-----+-----+-----+-----|       |  |       |-----+-----+-----+-----+-----+-----|
 * |  ▽  |     |     |     |     |     |-------|  |-------|     |     |     |     |     |     |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          |     |     |     |     | /       /     \      \ |     |     |     |     |
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */
[LAYER_PROG] = LAYOUT(
QK_BOOT,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,              KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,
DB_TOGG,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,              KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,
 KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,              KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,
_______,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,  KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,
              KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,  KC_NO,KC_NO,KC_NO,KC_NO,KC_NO)
};

#ifdef RGBLIGHT_ENABLE
char layer_state_str[72];
// Now define the array of layers. Later layers take precedence

// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_base_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_RED),SET_BACKLIGHT(HSV_RED),SET_THUMBS(HSV_RED));
// Light on outer column and underglow
const rgblight_segment_t PROGMEM layer_lower_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_TEAL),SET_BACKLIGHT(HSV_TEAL),SET_THUMBS(HSV_TEAL));
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_arrows_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_BLUE),SET_BACKLIGHT(HSV_BLUE),SET_THUMBS(HSV_BLUE));
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_prog_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_TEAL),SET_BACKLIGHT(HSV_TEAL),SET_THUMBS(HSV_TEAL));
const rgblight_segment_t PROGMEM layer_math_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    SET_INDICATORS(HSV_ORANGE),SET_BACKLIGHT(HSV_ORANGE),SET_THUMBS(HSV_ORANGE));

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer_base_lights,
    layer_lower_lights,
    layer_arrows_lights,
    layer_math_lights,
    layer_prog_lights
);

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, LAYER_BASE));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, LAYER_BASE));
    rgblight_set_layer_state(1, layer_state_cmp(state, LAYER_LOWER));
    rgblight_set_layer_state(2, layer_state_cmp(state, LAYER_ARROWS));
    rgblight_set_layer_state(3, layer_state_cmp(state, LAYER_LOWER) && layer_state_cmp(state, LAYER_ARROWS));
    rgblight_set_layer_state(4, layer_state_cmp(state, LAYER_PROG));
    return update_tri_layer_state(state, LAYER_LOWER, LAYER_ARROWS, LAYER_MATH);
}

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
    //rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);// haven't found a way to set this in a more useful way
}
#endif

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };
    oled_write_P(qmk_logo, false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n"), false);
    oled_write_ln_P(PSTR("Zander Erasmus"), false);
    oled_write_P(PSTR("\nLAYER: "), false);
    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE:
            oled_write_P(PSTR("Base "), false);
            break;
        case LAYER_LOWER:
            oled_write_P(PSTR("Lower"), false);
            break;
        case LAYER_ARROWS:
            oled_write_P(PSTR("Arrow"), false);
            break;
        case LAYER_MATH:
            oled_write_P(PSTR("Math "), false);
            break;
        case LAYER_PROG:
            oled_write_P(PSTR("Prog "), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_left()) {
        return OLED_ROTATION_0;
    } else {
        return OLED_ROTATION_0;
    }
}

bool oled_task_user(void) {
    if (is_keyboard_left()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    return false;
}

#endif

void sethsv(uint8_t hue, uint8_t sat, uint8_t val, int index);
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_EPIPE: {
            if (record->event.pressed) {
                SEND_STRING("|>");
                return false;
            }
        } break;
        case KC_VIMQ: {
            if (record->event.pressed) {
                rgblight_sethsv_at(HSV_RED, LEDPOS[KC_Q]);
                tap_code16(KC_ESC);
                SEND_STRING(":q");
                return false;
            }
        } break;
        case KC_VIMW: {
            if (record->event.pressed) {
                rgblight_sethsv_at(HSV_RED, LEDPOS[KC_W]);
                tap_code16(KC_ESC);
                SEND_STRING(":w");
                return false;
            }
        } break;
        case KC_VIME: {
            if (record->event.pressed) {
                rgblight_sethsv_at(HSV_RED, LEDPOS[KC_E]);
                tap_code16(KC_ESC);
                SEND_STRING(":e");
                return false;
            }
        } break;
        case KC_VIMS: {
            if (record->event.pressed) {
                rgblight_sethsv_at(HSV_RED, LEDPOS[KC_S]);
                SEND_STRING(":s/");
                return false;
            }
        } break;
        case KC_VCPY: {
            if (record->event.pressed) {
                SEND_STRING("\"+y");
                return false;
            }
        } break;
        case KC_VIMV: {
            if (record->event.pressed) {
                register_code16(KC_LCTL);
                SEND_STRING("r");
                unregister_code16(KC_LCTL);
                SEND_STRING("\"");
                return false;
            }
        } break;
        case KC_LARR: {
            if (record->event.pressed) {
                SEND_STRING("<-");
                return false;
            }
        } break;
        case KC_RARR: {
            if (record->event.pressed) {
                SEND_STRING("->");
                return false;
            }
        } break;
        case KC_LBIN: {
            if (record->event.pressed) {
                SEND_STRING("<<");
                return false;
            }
        } break;
        case KC_RBIN: {
            if (record->event.pressed) {
                SEND_STRING(">>");
                return false;
            }
        } break;
        case KC_LAEQ: {
            if (record->event.pressed) {
                SEND_STRING("<=");
                return false;
            }
        } break;
        case KC_RAEQ: {
            if (record->event.pressed) {
                SEND_STRING("=>");
                return false;
            }
        } break;
        default: {
            if ((keycode < 128) && (LEDPOS[keycode] != 0)) {
                //sethsv(HSV_RED, LEDPOS[keycode]);
            }
        } break;
    }
    return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_MS_WH_DOWN);
        } else {
            tap_code(KC_MS_WH_UP);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return false;
}

#endif
