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

#pragma once

typedef struct vector {
  float x;
  float y;
} vector_t;

typedef struct physics_state {
  vector_t accel;
  vector_t velocity;
} physics_state_t;

typedef struct physics_config {
  float force;
  float mass;
  float friction;
} physics_config_t;

#ifndef MOUSEKEY_FRAMERATE
#define MOUSEKEY_FRAMERATE 60.0
#endif
#ifndef MOUSEKEY_GRAVITY
#define MOUSEKEY_GRAVITY 9.8
#endif
#ifndef MOUSEKEY_CURSOR_FORCE
#define MOUSEKEY_CURSOR_FORCE 1100
#endif
#ifndef MOUSEKEY_CURSOR_MASS
#define MOUSEKEY_CURSOR_MASS 1
#endif
#ifndef MOUSEKEY_CURSOR_FRICTION
#define MOUSEKEY_CURSOR_FRICTION 95
#endif
#ifndef MOUSEKEY_WHEEL_FORCE
#define MOUSEKEY_WHEEL_FORCE 3
#endif
#ifndef MOUSEKEY_WHEEL_MASS
#define MOUSEKEY_WHEEL_MASS .2
#endif
#ifndef MOUSEKEY_WHEEL_FRICTION
#define MOUSEKEY_WHEEL_FRICTION 5
#endif

#ifdef __cplusplus
extern "C" {
#endif

void mousekey_task(void);
void mousekey_on(uint8_t code);
void mousekey_off(uint8_t code);
void mousekey_clear(void);
void mousekey_send(void);

#ifdef __cplusplus
}
#endif
