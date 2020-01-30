#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layers {
  L_DEFAULT= 0,
  L_XLL,
  L_XLM,
  L_XLR,
  L_XRL,
  L_XRM,
  L_XRR,
  L_XLM_XRM,
};

enum custom_keycodes {
  DEFAULT = SAFE_RANGE,
  XLR,
  XLM,
  XRL,
  XRM,
  XRR,
  BACKLIT,
  RGBRST
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO

#define KC_XLL KC_LALT
#define KC_XLM XLM //LT(L_XLM, KC_SPACE)
#define KC_XLR MT(MOD_LGUI, KC_LANG2)
#define KC_XRL MT(MOD_RGUI, KC_LANG1)
#define KC_XRM XRM
#define KC_XRR   LT(L_XRR, KC_F7)
#define KC_RST   RESET
#define KC_LRST  RGBRST
#define KC_LTOG  RGB_TOG
#define KC_LHUI  RGB_HUI
#define KC_LHUD  RGB_HUD
#define KC_LSAI  RGB_SAI
#define KC_LSAD  RGB_SAD
#define KC_LVAI  RGB_VAI
#define KC_LVAD  RGB_VAD
#define KC_LMOD  RGB_MOD
#define KC_CTLTB CTL_T(KC_TAB)
#define KC_CMD_S CTL_T(KC_TAB)
#define KC_LG(kc) LGUI(KC_##kc)
#define KC_LS(kc) LSFT(KC_##kc)
#define KC_CU KC_UP
#define KC_CD KC_DOWN
#define KC_CL KC_LEFT
#define KC_CR KC_RIGHT
#define KC_PU KC_PGUP
#define KC_PD KC_PGDN
#define KC_MCTL LCA(KC_F3)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_DEFAULT] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC,     X,     W,     L,     Z,     J,                      Q,     G,     H,     C,     Y,  SLSH,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      CTLTB,     O,     I,     E,     A,     F,                      B,     T,     R,     N,     S,  MINS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,  SCLN,   DOT,     K,     U,    AT,                      D,     M,     P,     V,  COMM,  RSFT,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                    XLL,   XLM,   XLR,      XRL,   XRM,   XRR \
                              //`--------------------'  `--------------------'
  ),

  [L_XLM] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC, XXXXX, XXXXX,  MS_U, XXXXX, XXXXX,                  XXXXX,  AMPR,  RPRN,   GRV,  ASTR,  QUES,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       MCTL, XXXXX,  MS_L,  MS_D,  MS_R, MS_BTN3,                QUOTE,  LPRN,  LCBR,  RCBR,   EQL,  UNDS,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,   BTN2,  WH_D, WH_U,  BTN1, XXXXX,                     AT,   DQT,  PLUS,  HASH,  EXLM, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,   _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),

  [L_XLR] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      _____, _____, _____, _____, _____, _____,                 _____, _____, _____, _____, _____, _____,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, _____, _____, _____, _____, _____,                 _____, _____, _____, _____, _____, _____,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, _____, _____, _____, _____, _____,                 _____, _____, _____, _____, _____, _____,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,   _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),

  [L_XRL] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
      _____, _____, _____, _____, _____, _____,                 _____, _____, _____, _____, _____, _____,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, _____, _____, _____, _____, _____,                 _____, _____, _____, _____, _____, _____,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      _____, _____, _____, _____, _____, _____,                 _____, _____, _____, _____, _____, _____,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,   _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),

  [L_XRM] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        ESC, XXXXX, XXXXX,    UP, XXXXX, XXXXX,                   XXXXX,  AMPR,  HOME, PGUP, XXXXX,  BSPC,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
      CTLTB, XXXXX,  LEFT,  DOWN, RIGHT, XXXXX,                   BSPC,   DEL,   END,  PGDN,   ENT,   GRV,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT, XXXXX,  COLN, XXXXX, XXXXX, XXXXX,                   UNDS,  PLUS,  LBRC,  RBRC,  LSFT,  TILD,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,   _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),

  [L_XRR] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        RST, XXXXX, XXXXX,    UP, XXXXX, XXXXX,                  XXXXX,  KP_7,  KP_8,  KP_9, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LTOG,  LHUI,  LEFT,  DOWN, RIGHT, XXXXX,                   BSPC,  KP_4,  KP_5,  KP_6, XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LSFT,  LHUD,   DOT,  LVAD, XXXXX, XXXXX,                   KP_0,  KP_1,  KP_2,  KP_3, XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,   _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),

  [L_XLM_XRM] = LAYOUT_kc( \
  //,-----------------------------------------.                ,-----------------------------------------.
        RST,  LRST, XXXXX,LS(CU), XXXXX, XXXXX,                  XXXXX, XXXXX,LS(HOME),LS(PU), XXXXX, XXXXX,\
      //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LTOG,  LHUI,LS(CL),LS(CD),LS(CR), XXXXX,                  XXXXX, XXXXX,LS(END),LS(PD), XXXXX, XXXXX,\
  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
       LMOD,  LHUD,  LSAD,  LVAD, XXXXX, XXXXX,                  XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX,\
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                  _____, _____, _____,   _____, _____, _____ \
                              //`--------------------'  `--------------------'
  ),
};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool layer_on_if_need(uint8_t layer){
  if(!IS_LAYER_ON(layer)){
    layer_on(layer);
    return true;
  }
  return false;
}
bool layer_off_if_need(uint8_t layer){
  if(IS_LAYER_ON(layer)){
    layer_off(layer);
    return true;
  }
  return false;
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on_if_need(layer3);
  } else {
    layer_off_if_need(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
//const char *read_keylogs(void);

const char *read_mode_icon(bool swap);
const char *read_host_led_state(void);
void set_timelog(void);
const char *read_timelog(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
//    matrix_write_ln(matrix, read_keylog());
    matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
//    matrix_write_ln(matrix, read_host_led_state());
    matrix_write_ln(matrix, read_layer_state());
//    matrix_write_ln(matrix, read_timelog());
  } else {
//    matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
//    matrix_write_ln(matrix, read_host_led_state());
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

uint16_t _lastKeyCode;
bool _isOneShot;
bool isOneShot(void){  return _isOneShot; }
bool process_record_user_wrapped(uint16_t keycode, keyrecord_t *record);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
//    set_keylog(keycode, record);
    set_timelog();
#endif
  }

  _isOneShot=_lastKeyCode==keycode;
  bool result = process_record_user_wrapped(keycode,record);
  _lastKeyCode=keycode;

  return result;
}

bool isShifted(void) {
 return get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT));
}

bool isControl(void) {
 return get_mods() & (MOD_BIT(KC_LCTRL) | MOD_BIT(KC_RCTRL));
}

void registerOrUnRegister(uint16_t keycode, bool isRegister){
  isRegister ? register_code(keycode) : unregister_code(keycode);
}

bool process_record_user_wrapped(uint16_t keycode, keyrecord_t *record) {

  bool isPressed = record->event.pressed;

  switch (keycode) {
    case DEFAULT:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<L_DEFAULT);
      }
      return false;
      break;

    case KC_XLM:
      record->event.pressed ? layer_on_if_need(L_XLM) : layer_off_if_need(L_XLM);
      update_tri_layer_RGB(L_XRM, L_XLM, L_XLM_XRM);

      if( !isPressed && isOneShot() ) {
          if(IS_LAYER_ON(L_XRM)) {
              register_code(KC_LANG2);
              unregister_code(KC_LANG2);
              return false;
          } else if( !layer_state_is(L_XLM) ) {
              register_code(KC_SPACE);
              unregister_code(KC_SPACE);
              return false;
          }
      }
      return true;
    case KC_XLR:
      record->event.pressed ? layer_on_if_need(L_XLR) : layer_off_if_need(L_XLR);
      break;
    case KC_XRM:
      record->event.pressed ? layer_on_if_need(L_XRM) : layer_off_if_need(L_XRM);
      update_tri_layer_RGB(L_XRM, L_XLM, L_XLM_XRM);

      if(IS_LAYER_ON(L_XLM) && !isPressed && isOneShot()){
         register_code(KC_LANG1);
         unregister_code(KC_LANG1);
         return false;
      }
      break;
    case KC_XRL:
        record->event.pressed ? layer_on_if_need(L_XRL) : layer_off_if_need(L_XRL);
        break;
//    case KC_XRR:
//      record->event.pressed ? layer_on_if_need(L_XRR) : layer_off_if_need(L_XRR);
//      update_tri_layer_RGB(L_XLM, L_XRM, L_XLM_XRM);
//      return false;
//      break;
//    case KC_XLM_XRM:
//        record->event.pressed ? layer_on(L_XLM_XRM) : layer_off(L_XLM_XRM);
//        update_tri_layer_RGB(L_XLM, L_XRM, L_XLM_XRM);
//      return false;
//      break;
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;

    case KC_RPRN: // >
        if (isShifted() && isPressed) { registerOrUnRegister(KC_DOT, true); return false; }
        else { registerOrUnRegister(KC_DOT, false); }
       break;
    case KC_LPRN: // <
      if (isShifted() && isPressed) { registerOrUnRegister(KC_COMMA, true); return false; }
      else { registerOrUnRegister(KC_COMMA, false); }
       break;

    case KC_LCBR: // [
      if (isShifted() && isPressed) {
        isPressed ? del_mods(MOD_BIT(KC_LSFT)) : 0;
        registerOrUnRegister(KC_LBRACKET, true);
        add_mods(MOD_BIT(KC_LSFT));
        return false;
      } else {
        registerOrUnRegister(KC_LBRACKET, false);
      }
      break;

    case KC_RCBR: // ]
      if (isShifted() && isPressed) {
        isPressed ? del_mods(MOD_BIT(KC_LSFT)) : 0;
        registerOrUnRegister(KC_RBRACKET, true);
        add_mods(MOD_BIT(KC_LSFT));
        return false;
      } else {
        registerOrUnRegister(KC_RBRACKET, false);
      }
      break;
    case KC_QUOTE: // %
        if (isShifted() && isPressed) {
          registerOrUnRegister(KC_5,true);
          return false;
        } else {
          registerOrUnRegister(KC_5,false);
        }
        break;
    case KC_AT: // backslash
        if (isShifted() && isPressed) {
          del_mods(MOD_BIT(KC_LSFT));
          registerOrUnRegister(KC_BSLS, true);
          add_mods(MOD_BIT(KC_LSFT));
          return false;
        } else {
          registerOrUnRegister(KC_BSLS, false);
        }
        break;
    case KC_PLUS: // $
        if (isShifted() && isPressed) {
          registerOrUnRegister(KC_DOLLAR, true);
          return false;
        } else {
          registerOrUnRegister(KC_DOLLAR, false);
        }
        break;
    case KC_HASH: // ^
        if (isShifted() && isPressed) {
          registerOrUnRegister(KC_CIRC, true);
          return false;
        } else {
          registerOrUnRegister(KC_CIRC, false);
        }
        break;
    case KC_AMPR: // |
        if (isShifted() && isPressed) {
          registerOrUnRegister(KC_PIPE, true);
          return false;
        } else {
          registerOrUnRegister(KC_PIPE, false);
        }
        break;
    case KC_KP_4:
    case KC_KP_6:
        if (isControl()) {
          add_mods(MOD_BIT(KC_LALT)); add_mods(MOD_BIT(KC_LCTRL));
          registerOrUnRegister(keycode, isPressed);
          del_mods(MOD_BIT(KC_LALT));
          return false;
        }
    case KC_KP_0:
    case KC_KP_1:
    case KC_KP_2:
    case KC_KP_3:
    case KC_KP_5:
    case KC_KP_7:
    case KC_KP_8:
    case KC_KP_9:
        if (IS_LAYER_ON(L_XLM)) {
          add_mods(MOD_BIT(KC_LALT)|MOD_BIT(KC_LCTRL));
          registerOrUnRegister(keycode, isPressed);
          del_mods(MOD_BIT(KC_LALT)|MOD_BIT(KC_LCTRL));
          return false;
        }
        break;
//    case KC_CTLTB:
//        if (IS_LAYER_ON(L_XLM) && isOneShot() && !isPressed) {
//          registerOrUnRegister(KC_LALT, true);
//          registerOrUnRegister(KC_F3, true);
//          registerOrUnRegister(KC_F3, false);
//          registerOrUnRegister(KC_LALT, false);
//          registerOrUnRegister(KC_LCTRL, false);
//          return false;
//        } else if (IS_LAYER_ON(L_XLR) && isOneShot() && !isPressed) {
//          add_mods(MOD_BIT(KC_LGUI)|MOD_BIT(KC_LCTRL));
//          registerOrUnRegister(KC_TAB, true);
//          registerOrUnRegister(KC_TAB, false);
//          del_mods(MOD_BIT(KC_LGUI)|MOD_BIT(KC_LCTRL));
//          return false;
//        }
  }
  return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
