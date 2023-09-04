#include "action.h"
#include "keyboard.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "oled_driver.h"
#include "quantum.h"
#include "timer.h"
#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "transactions.h"

#define OLED_TIMEOUT_OFF ((uint32_t)5*60*1000)

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
 * |-----+-----+-----+-----+-----+-----|  PREV |  | NEXT  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |  ▽  |  ▽  | <<  |  >> |  ▽  |-------|  |-------|  ▽  |  ▽  | <-  |  -> |  \  |  ▽  |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          |  ▽  |  ▽  |  ▽  |  ▽  | /   ▽   /     \  ▽   \ |  ▽  |  ▽  |  ▽  |  ▽  |
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */
[LAYER_LOWER] = LAYOUT(
KC_PSCR,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,                      KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,
_______,KC_EXLM,KC_AT,KC_HASH,KC_DLR,KC_PERC,           KC_CIRC,KC_AMPR,KC_ASTR,KC_PPLS,_______, _______,
_______,KC_TILD,KC_PIPE,KC_LPRN,KC_RPRN,KC_UNDS,      KC_EPIPE,KC_LBRC,KC_RBRC,KC_MINS,KC_EQL,KC_F12,
_______,_______,_______,KC_LBIN,KC_RBIN,_______,KC_MPRV,  KC_MNXT,_______,_______,KC_LARR,KC_RARR,KC_BSLS,_______,
             _______,_______,_______,_______,_______,  _______,_______,_______,_______,_______),
/* ARROWS 2
 * ,-----------------------------------.                  ,-----------------------------------.
 * |PRNTS|  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |                  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  | :q  | :w  | :e  |MENU |PRNTS|                  | PGU |Home |  ⬆  | End |     |  ▽  |
 * |-----+-----+-----+-----+-----+-----|                  |-----+-----+-----+-----+-----+-----|
 * |  ▽  |     | :s/ |  {  |  }  |  %  |-------.  ,-------| PGD |  ←  |  ↓  |  →  |     |  ▽  |
 * |-----+-----+-----+-----+--=--+-----|  ▽    |  |    ▽  |-----+--=--+-----+-----+-----+-----|
 * |  ▽  |     |     |VCOPY| CR" |     |-------|  |-------|     |CTHom|     |CTEnd| Ins |  ▽  |
 * `-----------------------------------/       /   \      \-----------------------------------'
 *          |  ▽  |  ▽  |  ▽  |  ▽  | /   ▽   /     \  ▽   \ |  ▽  |  ▽  |  ▽  |  ▽  |
 *          |     |     |     |     |/       /       \      \|     |     |     |     |
 *          `-------------------------------'         '------'-----------------------'
 */
[LAYER_ARROWS] = LAYOUT(
KC_PSCR,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,                          KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,
_______,KC_VIMQ,KC_VIMW,KC_VIME,KC_APP,KC_PSCR,                 KC_PGUP,KC_HOME,KC_UP,KC_END,KC_NO,_______,
_______,KC_NO,KC_VIMS,KC_LCBR,KC_RCBR,KC_PERC,            KC_PGDN,KC_LEFT,KC_DOWN,KC_RGHT,KC_NO,KC_F12,
_______,_______,_______,KC_VCPY,KC_VIMV,KC_NO,_______,   _______,KC_NO,LCTL(KC_HOME),KC_NO,LCTL(KC_END),KC_INS,_______,
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

#ifdef OLED_ENABLE

enum {
    SQUID_PASSIVE_1 = 0,
    SQUID_PASSIVE_2,
    SQUID_PASSIVE_3,
    SQUID_PASSIVE_4,
    SQUID_PASSIVE_5,
    SQUID_PASSIVE_6,
    SQUID_PASSIVE_7,
    SQUID_PASSIVE_8,
    SQUID_PASSIVE_9,
    SQUID_PASSIVE_10,
    SQUID_PASSIVE_11,
    SQUID_PASSIVE_12,
    SQUID_PASSIVE_13,
    SQUID_PASSIVE_14,
    SQUID_PASSIVE_15,
    SQUID_PASSIVE_16,

    SQUID_NUM_FRAMES
};

#define SQUID_SIZE (32*32/8)
#define ANIM_FRAME_TIME 250

static void render_squid(uint8_t frame) {
    static const unsigned char PROGMEM raw_logo[SQUID_NUM_FRAMES][SQUID_SIZE] = {
        [SQUID_PASSIVE_1] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,  4,  2,225,241,144,224,  0,  0,224,240,145,225,  2,  4, 24,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 96, 23,  8,241,  0,  0,101,169, 40,232, 40,104, 73,133,  0, 32,209,  8, 23, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,  4,  3,  0,  7,  8, 16,  9, 14, 16, 35, 36, 40, 17, 30, 32, 32, 23,  8,  4,  3,  0,  0,  0,  0,  0},
        [SQUID_PASSIVE_2] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,144,  4,224, 25,  4,  2,  1, 65,160,224, 64,  0,  0, 64,160,225, 65,  2,  4, 24,224,  0,  0,  0,  0,  0,  0,  0,  0,128, 96, 18,144, 72,231, 56,  0,  1,192, 36, 72, 72,200, 72,200,  8,  8, 68,192, 32,225, 24,  7,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  1, 14, 17, 16,  8,  7, 24, 32, 46, 17,  6, 24, 32, 71, 40, 16, 35, 60, 16, 19, 10,  4,  0,  0,  0,  0,  0},
        [SQUID_PASSIVE_3] = {0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16,132,  1,  0,128, 96, 24,  4,  2,  1, 65, 65, 64, 64,  0,  0, 64, 64, 64, 65,  2, 12, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,192, 32, 32,144,137,102, 24,  0,129, 64,100, 72,200, 72, 72, 72,200,  8,  4,192,  1,  0, 28,227,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  1,  8, 22, 17,  8,  4, 27, 32, 32, 23, 56, 79, 64, 32, 25,118, 64, 35, 28, 32, 31,  0,  0,  0,  0,  0,  0},
        [SQUID_PASSIVE_4] = {0,  0,  0,  0, 32,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  4,  0,  0,  0,224, 24,  4,  2, 65,161,224, 64,  0,  0, 64,160,225, 65,  2,  4, 24,224,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32,152,199, 40, 16,129, 64, 68, 72,200, 72, 72,200,  8,  4,  0,193, 16, 40,199,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  1, 13, 19, 16,  8, 22, 41, 32, 16, 46, 41, 38, 16, 56, 77, 34, 32, 24, 23, 16,  8,  7,  0,  0,  0,  0},
        [SQUID_PASSIVE_5] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,  4,  2,225, 49,240,224,  0,  0,224, 48,241,225,  2,  4, 24,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,199, 56, 33,192,  0,  5,201, 72, 72,200, 72, 73,133,  0, 32,209,  8, 23, 32,192,  0,  0,  0,  0,  0,  0,  0,  0,  6,  9,  9,  8,  4, 28, 35, 32, 24, 39, 32, 92, 67, 88, 39, 32, 17, 46, 32, 32, 23,  8,  8,  4,  3,  0,  0},
        [SQUID_PASSIVE_6] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,  4,  2,225, 49,240,224,  0,  0,224, 48,241,225,  2,  4, 24,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 71, 40,145, 96,  0,  5,201, 72,200, 72,200,  9,  5,  0, 96,145, 40, 71, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 19, 16, 12, 18, 33, 32, 28, 35, 64, 76, 51, 16, 32, 23,  8, 16, 32, 33, 18, 12, 16,  9,  6,  0,  0},
        [SQUID_PASSIVE_7] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,132,  2,225,145,240,224,  0,  0,224,144,241,225,  2,132, 24,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32,144, 83, 44, 16,  0,128, 66, 68, 68,196, 68,196,  4,  2,  0, 96,144, 12, 19, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,  4,  7,  8, 16, 16, 15, 16, 16, 14,  9, 16, 16,  8,  7,  4,  8, 16, 17, 14,  4,  4,  3,  0,  0,  0},
        [SQUID_PASSIVE_8] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128, 64, 64, 32, 32, 32, 32, 64, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 28, 66,  1, 56, 72,120, 56,  0,  0, 56, 72,120, 56,  0, 65,  6, 56,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 32,144, 75, 36, 24,128, 65, 34,146,114, 20, 20,242,  2,  1,224, 16, 44,195,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  4,  4, 11, 16, 16, 14, 17, 16,  8,  7,  8, 16, 16, 14,  3, 12, 16, 15,  4,  4,  3,  0,  0,  0,  0},
        [SQUID_PASSIVE_9] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 32, 16, 16, 16,  8,  8,  8,  8,  8, 16, 16, 32, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,124,131, 32,  0, 28,178, 62, 28,  0,  0, 28, 50, 62,156,  0, 32,  0,195,124,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 32,156, 67, 60,128,112,  8,  9,  9,234, 18, 18, 18,225,  0,  0,248,  7, 56,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8, 11, 12, 16,  9,  6,  8,  8,  7,  8, 16,  8,  7,  8,  4,  3,  2,  4,  4,  3,  0,  0,  0},
        [SQUID_PASSIVE_10] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 16, 16,  8,  8,  8,  8,  8,  8, 16, 16, 32, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,254,  1, 32,  0, 28,166, 62, 28,  0,  0, 28, 38,190, 28,  0, 32,  1,254,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,196, 50, 13,  2,192, 32, 16,  9,201, 57,  9, 17, 17,224,  0,  4, 26, 97,130, 12,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  1, 16,  1,  2,  2,  1, 18,  4,  2,  1,  2,  4,  4, 19,  0,  3,  4,  4, 18,  1,  1,  4,  0},
        [SQUID_PASSIVE_11] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 16, 16,  8,  8,  8,  8,  8,  8, 16, 16, 32, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,254,  1, 32,  0, 28,166, 62, 28,  0,  0, 28, 38,190, 28,  0, 32,  1,254,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 76, 66,113,136,  5,  2,224, 16,  8,133,197, 61,  5,  5,137,112,128,  0,  6, 25,225,134,120,  0,  0,  0,  0,  0,  0,  1,  0,  0,  2,  0,  0,  0,  1,  5,  0,  1,  1,  0,  0,  1,  2,  2,  1,  0,  1,  2,  2,  1,  4,  0,  0,  1},
        [SQUID_PASSIVE_12] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 16, 16,  8,  8,  8,  8,  8,  8, 16, 16, 32, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,254,  1, 32,  0, 28,166, 62, 28,  0,  0, 28, 38,190, 28,  0, 32,  1,254,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 76, 66,113,136,  5,  2,224, 16,  8,133,197, 61,  5,  5,137,112,128,  0,  6, 25,225,134,120,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  2,  2,  1,  0,  1,  2,  2,  1,  0,  0,  0,  0},
        [SQUID_PASSIVE_13] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,144, 64, 64, 32, 32, 40, 32, 32, 32, 64,144,  0, 64,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,224, 24,  6, 33,  0, 24, 44, 60, 24,  0,  0, 24, 44, 60, 24,  1, 34,  5,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 65, 64,240,  9,  6,224, 16,  8,233, 25,  9,  9,201, 57,  9,  0,  0,120,132, 67, 66, 68, 56,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  1,  2,  2,  1,  1,  2,  2,  2,  1,  1,  0,  0,  0,  0,  0},
        [SQUID_PASSIVE_14] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,128,128,128,128,128,161,  0,  0,  0,  0,132,  0,  8, 64,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,  4, 66, 17, 17, 17, 16,  0,  0, 16, 16, 16, 17, 65,  1,  2,  4, 24,224,  8,  0,  0,  0,  0,  0,  0,  0,  0, 24, 36, 34, 81,142,128, 64, 48, 18, 18,242, 18, 18,210, 50, 16,  0,  0,224, 80,152,132,139,144, 96,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  1,  1,  1,  2,  2,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        [SQUID_PASSIVE_15] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  8,  0,  0,  0, 64,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,132,  4, 66,162,225, 65,  1,  1, 65,161,225, 66,  2,130,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 75, 68,168, 16,  0,192, 34, 34, 36,228, 36, 36,226, 34,  0,  0,  0,224, 16,  8,151, 96,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  2,  2,  2,  1,  1,  2,  2,  1,  2,  4,  4,  3,  0,  1,  1,  1,  0,  0,  0,  0,  0},
        [SQUID_PASSIVE_16] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,240,  8,  4,  2,  1,193, 33,224,192,  0,  0,192, 32,224,193,  1,  2,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,112,136,  4,227, 20,  8,  1,128, 64, 37, 37, 36,228, 36, 36,197,  5,  0,  0, 97,160, 16,  8, 23,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  2,  2,  2,  1,  2,  4,  4,  2,  5,  8,  8,  4,  3,  4,  8,  8,  4,  3,  4,  2,  1,  0,  0,  0,  0},
    };
    oled_write_raw_P((const char *)(raw_logo[frame]), SQUID_SIZE);
}

#define SQUID_TYPING_SIZE (32*24/8)

enum {
    SQUID_TYPING_PAUSE_1 = 0,
    SQUID_TYPING_PAUSE_2,
    SQUID_TYPING_PAUSE_3,
    SQUID_TYPING_PAUSE_FRAMES
};

enum {
    SQUID_TYPING_1 = 0,
    SQUID_TYPING_2,
    SQUID_TYPING_3,
    SQUID_TYPING_4,

    SQUID_TYPING_FRAMES
};

static const uint8_t PROGMEM frames_squid_typing_long_pause[SQUID_TYPING_SIZE] = {0,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,  4,194,226,225,193,  1,  1,193,225,226,194,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,159, 96,130,  0,  1, 10,146,145,144,144,145, 18, 10,  1,128,130, 96,159,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 38, 33, 24, 38, 65, 64, 56, 70,129,152,103, 32, 65, 46, 16, 32, 64, 67, 36, 24, 32, 19, 12,  0,  0,  0};
static const uint8_t PROGMEM frames_squid_typing_pause[SQUID_TYPING_PAUSE_FRAMES][SQUID_TYPING_SIZE] = {
    [SQUID_TYPING_PAUSE_1] = {0,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,  4,194,226,225,193,  1,  1,193,225,226,194,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 32, 16,223, 32, 18, 16, 33,194, 26,145,144,144,145, 26,194, 33, 16, 18, 32,223, 16, 32, 32, 64,128,  0,  0,  0,  7,  8,  8,  4,  2,  3,  4,  8,  8,  8,  4, 27, 33, 33, 30, 16, 16,  9,  7,  8,  8,  8,  4,  3,  1,  2,  4,  8,  8,  7},
    [SQUID_TYPING_PAUSE_2] = {0,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,  4,194,226,225,193,  1,  1,193,225,226,194,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32,223, 32, 18, 16, 33,202, 18,145,144,144,145, 18,202, 33, 16, 18, 32,223, 32, 32, 64,128,  0,  0,  0,  0, 14, 17, 16,  8,  4,  3,  4,  8,  8,  8,  4,  3, 13, 16, 17, 14, 16, 17, 23,  8,  8,  8,  4,  3,  1,  6,  8, 16, 17, 14},
    [SQUID_TYPING_PAUSE_3] = {0,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,  4,130,130,129,129,  1,  1,129,129,130,130,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 32, 16,223, 32, 17, 16, 32,196,  8,136,136,136,136,  8,196, 32, 16, 17, 32,223, 16, 32, 32, 64,128,  0,  0,  0,  7,  8,  8,  4,  2,  3,  4,  8,  8,  8,  4, 27, 33, 33, 30, 16, 16,  9,  7,  8,  8,  8,  4,  3,  1,  2,  4,  8,  8,  7},
};
static const uint8_t PROGMEM frames_squid_typing[SQUID_TYPING_FRAMES][SQUID_TYPING_SIZE] = {
    [SQUID_TYPING_1] = {0,  0,  0,  0,  0,  0,  0,  0,128, 96, 24,  4,194,226,225,193,  1,  1,193,225,226,194,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32,208, 63,  0,  4,  0,  1, 10, 18,145,144,144,145, 18,202, 33, 16, 18, 32,223, 32, 32, 64,128,  0,  0,  0,  0, 14, 17, 16,  8,  6,  1,  6, 56,192,128,248,  6, 13, 16, 17, 14, 16, 17, 23,  8,  8,  8,  4,  3,  1,  6,  8, 16, 17, 14},
    [SQUID_TYPING_2] = {0,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,  4,194,226,225,193,  1,  1,193,225,226,194,  4, 24, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32,223, 32, 18, 16, 33,202, 18,145,144,144,145, 18, 10,  1,  0,  4,  0, 31,224, 32, 64,128,  0,  0,  0,  0, 14, 17, 16,  8,  4,  3,  4,  8,  8,  8,  4,  3, 13, 16, 17, 14, 16, 17,126,128,128, 96, 24,  6,  1,  6,  8, 16, 17, 14},
    [SQUID_TYPING_3] = {0,  0,  0,  0,  0,  0,  0,  0,  0,224, 24,  4,194,226,225,193,  1,  1,193,225,226,194,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 32, 16,208, 63,  0,  4,  0,  1, 10, 18,145,144,144,145, 26,194, 33, 16, 18, 32,223, 16, 32, 64,128,  0,  0,  0,  0,  7,  8,  8,  6,  1,  3, 60,192,128,120, 12, 18, 33, 33, 30, 16, 16,  9,  7,  8,  8,  4,  2,  1,  2,  4,  8,  8,  7,  0},
    [SQUID_TYPING_4] = {0,  0,  0,  0,  0,  0,  0,  0,192, 48,  8,  4,194,226,225,193,  1,  1,193,225,226,194,  4,  8, 48,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 32, 32,223, 32, 18, 16, 33,194, 26,145,144,144, 17, 18, 10,  1,  0,  4,128,127, 16, 16, 32, 32, 64,128,  0,  0,  0, 15, 16, 16, 12,  3,  4,  8,  8,  8,  4, 27, 33, 33, 30, 16, 17, 14,240,128, 64, 56,  7,  1,  1,  1,  6,  8,  8,  7},
};

static uint32_t animation_timer = 0;
static void render_squid_idle(void){
    static uint8_t idleframe = 0;
    static uint8_t prev_idleframe = 0;

    if ((prev_idleframe == SQUID_PASSIVE_1) || (prev_idleframe == SQUID_PASSIVE_6) || (prev_idleframe == SQUID_PASSIVE_12)) {
        if (timer_elapsed32(animation_timer) > (5*ANIM_FRAME_TIME)) {
            animation_timer = timer_read32();
            render_squid(idleframe);
            prev_idleframe = idleframe;
            idleframe = (idleframe + 1) % 16;
        }
    } else {
        if (timer_elapsed32(animation_timer) > ANIM_FRAME_TIME) {
           animation_timer = timer_read32();
           render_squid(idleframe);
           prev_idleframe = idleframe;
           idleframe = (idleframe + 1) % 16;
        }
    }
}

static uint16_t typing_frame = 0;
static uint32_t active_timer = 0;
static uint32_t time_offset = 0;
static void squid_typing_typechar(uint16_t keycode){
    typing_frame = (typing_frame + timer_read() + keycode + 1) % SQUID_TYPING_FRAMES;
}

static uint8_t squid_typing_pause(void){
#if defined(__AVR_ATmega32U4__)
    return (TCNT0 + TCNT1 + TCNT3 + TCNT4) % SQUID_TYPING_PAUSE_FRAMES;
#else
    return rand() % SQUID_TYPING_PAUSE_FRAMES;
#endif
}

static void render_squid_keyboard(void) {
    static const unsigned char PROGMEM raw_logo[] = {
        145, 95,113,203,149,171,149,171,149,171,149,171,149,169,147,141,177,203,149,171,149,171,149,171,149,171,149,169,147,142,184,224,
    };
    oled_set_cursor(0, 4);
    oled_write_raw_P((const char *)(raw_logo), sizeof(raw_logo));
}

#define SQUID_TYPING_DELAY_IDLE (3500)
#define SQUID_TYPING_DELAY_LONG_PAUSE (3000)
#define SQUID_TYPING_DELAY_PAUSE (100)
#define SQUID_TYPING_DELAY_ANIM (13)
static void render_squid_typing(void){
    render_squid_keyboard();
    oled_set_cursor(0, 0);
    uint32_t lastactivityelapsed = timer_elapsed32(active_timer);
    if ((lastactivityelapsed > SQUID_TYPING_DELAY_IDLE)) {
        render_squid_idle();
        return;
    }
    if ((lastactivityelapsed > SQUID_TYPING_DELAY_LONG_PAUSE)) {
        if (timer_elapsed32(animation_timer) > SQUID_TYPING_DELAY_ANIM) {
            animation_timer = timer_read32();
            oled_write_P(PSTR("     "), false);
            oled_write_raw_P((const char *)(frames_squid_typing_long_pause), SQUID_TYPING_SIZE);
        }
        return;
    }
    if ((lastactivityelapsed > SQUID_TYPING_DELAY_PAUSE)) {
        if ( timer_elapsed32(animation_timer) <= SQUID_TYPING_DELAY_PAUSE) return;
        if ((lastactivityelapsed < 3 * SQUID_TYPING_DELAY_PAUSE) || (timer_elapsed32(animation_timer) > 3 * SQUID_TYPING_DELAY_PAUSE)) {
            animation_timer = timer_read32();
            oled_write_P(PSTR("     "), false);
            oled_write_raw_P((const char *)(frames_squid_typing_pause[squid_typing_pause()]), SQUID_TYPING_SIZE);
        }
        return;
    }
    if (timer_elapsed32(animation_timer) > SQUID_TYPING_DELAY_ANIM) {
        animation_timer = timer_read32();
        oled_write_P(PSTR("     "), false);
        oled_write_raw_P((const char *)(frames_squid_typing[typing_frame]), SQUID_TYPING_SIZE);
    }
    return;
}

static void print_status_narrow(void) {
    // Print current layer
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case LAYER_LOWER:
            oled_write_P(PSTR("Lower"), false);
            break;
        case LAYER_ARROWS:
            oled_write_P(PSTR("Arrow"), false);
            break;
        case LAYER_MATH:
            oled_write_P(PSTR("Math\n"), false);
            break;
        case LAYER_PROG:
            oled_write_P(PSTR("Prog\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_270;
    }

    return rotation;
}

bool oled_task_user(void) {
    if ((last_input_activity_elapsed() > OLED_TIMEOUT_OFF)) {
        if (is_oled_on()) {
            oled_off();
        }
        return false;
    }
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_squid_typing();
    }
    return false;
}

#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, LAYER_LOWER, LAYER_ARROWS, LAYER_MATH);
}

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    typing_frame = *((uint16_t*)in_data);
}

void user_sync_b_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    active_timer = *((uint32_t*)in_data) + time_offset;
}

static uint32_t mastertime = 0;
void user_sync_c_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    mastertime = *((uint32_t*)in_data);
    time_offset = timer_read32() - mastertime;
}

static uint16_t last_sent_frame = 0;
static uint32_t last_sent_active = 0;
static uint32_t last_sent_mastertime = -60000;
void housekeeping_task_user(void) {
    if (!is_keyboard_master()) return;

    if (last_sent_frame != typing_frame) {
        if (transaction_rpc_send(USER_SYNC_A, sizeof(typing_frame), &typing_frame)) {
            last_sent_frame = typing_frame;
        }
    }

    if (last_sent_active != active_timer) {
        if (transaction_rpc_send(USER_SYNC_B, sizeof(active_timer), &active_timer)) {
            last_sent_active = active_timer;
        }
    }

    if (timer_elapsed32(last_sent_mastertime) > 60000) {
        mastertime = timer_read32();
        if (transaction_rpc_send(USER_SYNC_C, sizeof(mastertime), &mastertime)) {
            last_sent_mastertime = timer_read32();
        }
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
    transaction_register_rpc(USER_SYNC_B, user_sync_b_slave_handler);
    transaction_register_rpc(USER_SYNC_C, user_sync_c_slave_handler);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        squid_typing_typechar(keycode);
        active_timer = timer_read32();
    }
    switch (keycode) {
        case KC_EPIPE: {
            if (record->event.pressed) {
                SEND_STRING("|>");
                return false;
            }
        } break;
        case KC_VIMQ: {
            if (record->event.pressed) {
                tap_code16(KC_ESC);
                SEND_STRING(":q");
                return false;
            }
        } break;
        case KC_VIMW: {
            if (record->event.pressed) {
                tap_code16(KC_ESC);
                SEND_STRING(":w");
                return false;
            }
        } break;
        case KC_VIME: {
            if (record->event.pressed) {
                tap_code16(KC_ESC);
                SEND_STRING(":e");
                return false;
            }
        } break;
        case KC_VIMS: {
            if (record->event.pressed) {
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
    return true;
}
#endif

