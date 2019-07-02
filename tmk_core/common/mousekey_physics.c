/*
Copyright 2019 Masaru Nemoto <nemolize@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include "keycode.h"
#include "host.h"
#include "timer.h"
#include "print.h"
#include "debug.h"
#include "mousekey_physics.h"

static report_mouse_t mouse_report = {};

physics_state_t move_state  = {{0, 0}, {0, 0}};
physics_state_t wheel_state = {{0, 0}, {0, 0}};

static uint16_t last_timer = 0;

const physics_config_t move  = {
    .force = CONVERT_FROM_FLOAT(MOUSEKEY_CURSOR_FORCE),
    .mass = CONVERT_FROM_FLOAT(MOUSEKEY_CURSOR_MASS),
    .friction = CONVERT_FROM_FLOAT(MOUSEKEY_CURSOR_FRICTION_MUL_DT_GRAVITY_MASS)
};
const physics_config_t wheel = {
    .force = CONVERT_FROM_FLOAT(MOUSEKEY_WHEEL_FORCE),
    .mass = CONVERT_FROM_FLOAT(MOUSEKEY_WHEEL_MASS),
    .friction = CONVERT_FROM_FLOAT(MOUSEKEY_WHEEL_FRICTION_MUL_DT_GRAVITY_MASS)
};

FIXED_POINT_NUMBER apply_friction_1d(const physics_config_t *conf, FIXED_POINT_NUMBER velocity) {
  const int sign = (velocity > 0) - (velocity < 0);
  velocity -= sign * conf->friction; //
  const int afterSign = (velocity > 0) - (velocity < 0);
  if (sign != afterSign) {
    velocity = 0;
  }
  return velocity;
}

void apply_friction_2d(const physics_config_t *conf, vector_t *velocity) {
  velocity->x = apply_friction_1d(conf, velocity->x);
  velocity->y = apply_friction_1d(conf, velocity->y);
}

void mousekey_task(void) {
  if (timer_elapsed(last_timer) < interval) return;

  // move
  {
    move_state.velocity.x += FPN_MUL(move_state.accel.x , MOUSEKEY_CURSOR_DT_DIV_MASS);  // v=at, a = F/m
    move_state.velocity.y += FPN_MUL(move_state.accel.y , MOUSEKEY_CURSOR_DT_DIV_MASS);
    apply_friction_2d(&move, &move_state.velocity);
    mouse_report.x = CONVERT_TO_INT(move_state.velocity.x);
    mouse_report.y = CONVERT_TO_INT(move_state.velocity.y);
  }

  // wheel
  {
    wheel_state.velocity.x += FPN_MUL(wheel_state.accel.x, MOUSEKEY_WHEEL_DT_DIV_MASS);  // a = F/m
    wheel_state.velocity.y += FPN_MUL(wheel_state.accel.y, MOUSEKEY_WHEEL_DT_DIV_MASS);
    apply_friction_2d(&wheel, &wheel_state.velocity);
    mouse_report.h = CONVERT_TO_INT(wheel_state.velocity.x);
    mouse_report.v = CONVERT_TO_INT(wheel_state.velocity.y);
  }

  if (mouse_report.x == 0 && mouse_report.y == 0 && mouse_report.v == 0 && mouse_report.h == 0) return;

  mousekey_send();
}

void mousekey_on(uint8_t code) {
  if      (code == KC_MS_UP)       move_state.accel.y = -move.force;
  else if (code == KC_MS_DOWN)     move_state.accel.y = move.force;
  else if (code == KC_MS_LEFT)     move_state.accel.x = -move.force;
  else if (code == KC_MS_RIGHT)    move_state.accel.x = move.force;
  else if (code == KC_MS_WH_UP)    wheel_state.accel.y = wheel.force;
  else if (code == KC_MS_WH_DOWN)  wheel_state.accel.y = -wheel.force;
  else if (code == KC_MS_WH_LEFT)  wheel_state.accel.x = wheel.force;
  else if (code == KC_MS_WH_RIGHT) wheel_state.accel.x = -wheel.force;
  else if (code == KC_MS_BTN1)     mouse_report.buttons |= MOUSE_BTN1;
  else if (code == KC_MS_BTN2)     mouse_report.buttons |= MOUSE_BTN2;
  else if (code == KC_MS_BTN3)     mouse_report.buttons |= MOUSE_BTN3;
  else if (code == KC_MS_BTN4)     mouse_report.buttons |= MOUSE_BTN4;
  else if (code == KC_MS_BTN5)     mouse_report.buttons |= MOUSE_BTN5;
}

void mousekey_off(uint8_t code) {
  if      (code == KC_MS_UP       && move_state.accel.y < 0) move_state.accel.y = 0;
  else if (code == KC_MS_DOWN     && move_state.accel.y > 0) move_state.accel.y = 0;
  else if (code == KC_MS_LEFT     && move_state.accel.x < 0) move_state.accel.x = 0;
  else if (code == KC_MS_RIGHT    && move_state.accel.x > 0) move_state.accel.x = 0;
  else if (code == KC_MS_WH_UP    && wheel_state.accel.y > 0) wheel_state.accel.y = 0;
  else if (code == KC_MS_WH_DOWN  && wheel_state.accel.y < 0) wheel_state.accel.y = 0;
  else if (code == KC_MS_WH_LEFT  && wheel_state.accel.x < 0) wheel_state.accel.x = 0;
  else if (code == KC_MS_WH_RIGHT && wheel_state.accel.x > 0) wheel_state.accel.x = 0;
  else if (code == KC_MS_BTN1) mouse_report.buttons &= ~MOUSE_BTN1;
  else if (code == KC_MS_BTN2) mouse_report.buttons &= ~MOUSE_BTN2;
  else if (code == KC_MS_BTN3) mouse_report.buttons &= ~MOUSE_BTN3;
  else if (code == KC_MS_BTN4) mouse_report.buttons &= ~MOUSE_BTN4;
  else if (code == KC_MS_BTN5) mouse_report.buttons &= ~MOUSE_BTN5;
}

void mousekey_debug(void) {
  if (!debug_mouse) return;
  print("mousekey [btn|x y v h](rep/acl): [");
  phex(mouse_report.buttons); print("|");
  print_decs(mouse_report.x); print(" ");
  print_decs(mouse_report.y); print(" ");
  print_decs(mouse_report.v); print(" ");
  print_decs(mouse_report.h); print("](");
}

void mousekey_send(void) {
  mousekey_debug();
  host_mouse_send(&mouse_report);
  last_timer = timer_read();
}

void mousekey_clear(void) {
  mouse_report = (report_mouse_t){};
}
