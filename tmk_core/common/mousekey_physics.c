/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

typedef struct vector {
  float x;
  float y;
} vector_t;

static report_mouse_t mouse_report = {};

typedef struct physics_state {
  vector_t accel;
  vector_t velocity;
} physics_state_t;

physics_state_t move_state  = {{0, 0}, {0, 0}};
physics_state_t wheel_state = {{0, 0}, {0, 0}};

static void mousekey_debug(void);

static uint16_t last_timer = 0;
const float     gravity    = MOUSEKEY_GRAVITY;
const float     dt         = 1.0 / MOUSEKEY_FRAMERATE;
const float     interval   = 1000.0 / MOUSEKEY_FRAMERATE;

typedef struct physics_config {
  float force;
  float mass;
  float friction;
} physics_config_t;

const physics_config_t move  = {.force = MOUSEKEY_CURSOR_FORCE, .mass = MOUSEKEY_CURSOR_MASS, .friction = MOUSEKEY_CURSOR_FRICTION};
const physics_config_t wheel = {.force = MOUSEKEY_WHEEL_FORCE, .mass = MOUSEKEY_WHEEL_MASS, .friction = MOUSEKEY_WHEEL_FRICTION};

float apply_friction_1d(const physics_config_t *conf, float velocity) {
  const int sign = velocity == 0 ? 0 : (0 < velocity ? 1 : -1);
  velocity -= sign * (conf->mass) * gravity * (conf->friction) * dt;
  const int afterSign = velocity == 0 ? 0 : (0 < velocity ? 1 : -1);
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
    move_state.velocity.x += (move_state.accel.x / move.mass) * dt;  // a = F/m
    move_state.velocity.y += (move_state.accel.y / move.mass) * dt;
    apply_friction_2d(&move, &move_state.velocity);
    mouse_report.x = move_state.velocity.x;
    mouse_report.y = move_state.velocity.y;
  }

  // wheel
  {
    wheel_state.velocity.x += (wheel_state.accel.x / wheel.mass) * dt;  // a = F/m
    wheel_state.velocity.y += (wheel_state.accel.y / wheel.mass) * dt;
    apply_friction_2d(&wheel, &wheel_state.velocity);
    mouse_report.h = wheel_state.velocity.x;
    mouse_report.v = wheel_state.velocity.y;
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

void mousekey_send(void) {
  mousekey_debug();
  host_mouse_send(&mouse_report);
  last_timer = timer_read();
}

void mousekey_clear(void) {
  mouse_report = (report_mouse_t){};
}

static void mousekey_debug(void) {
  if (!debug_mouse) return;
  print("mousekey [btn|x y v h](rep/acl): [");
  phex(mouse_report.buttons); print("|");
  print_decs(mouse_report.x); print(" ");
  print_decs(mouse_report.y); print(" ");
  print_decs(mouse_report.v); print(" ");
  print_decs(mouse_report.h); print("](");
}
