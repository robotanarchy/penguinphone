#pragma once
/*
        This file is part of penguinphone.

        penguinphone is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        penguinphone is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with penguinphone.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include <time.h>

typedef struct bar_t bar_t;
typedef struct bar_entry_t {
  void (*call_predraw)(bar_t *bar, struct bar_entry_t *entry);
  void (*call_onclick)(bar_t *bar, struct bar_entry_t *entry);
  const char *icon;
  const char *text;

  // internal usage
  struct bar_entry_t *next;
  struct timespec last_pressed;
  bool just_pressed;

} bar_entry_t;

typedef struct bar_t {
  bar_entry_t *first_entry;
  float click_effect_duration;
  bool needs_redraw;
  void *userdata;
} bar_t;

bar_t *bar_init(float click_effect_duration, void *userdata);
bar_entry_t *bar_add_entry(bar_t *bar);
void bar_update(bar_t *bar);
void bar_cleanup(bar_t *bar);

// internal function
void bar_stdin_parse(bar_t *bar);
