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

#include "bar.h"
#include "../common/wm_ipc/wm_ipc.h"
#include "icons.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bar_t *bar_init(float click_effect_duration, void *userdata) {
  // make stdout unbuffered, stdin non-blocking
  // http://stackoverflow.com/q/717572/#comment533736_717586
  setbuf(stdout, NULL);
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

  // print out the i3bar protocol header
  printf("{ \"version\": 1, \"click_events\": true }\n");
  printf("[[]\n");

  bar_t *bar = calloc(1, sizeof(bar_t));
  bar->click_effect_duration = click_effect_duration;
  bar->userdata = userdata;
  return bar;
}

bar_entry_t *bar_add_entry(bar_t *bar) {
  bar_entry_t *ret = calloc(1, sizeof(bar_entry_t));

  if (bar->first_entry) {
    bar_entry_t *previous = bar->first_entry;
    while (previous->next)
      previous = previous->next;
    previous->next = ret;
  } else {
    bar->first_entry = ret;
  }
  bar->needs_redraw = true;
  return ret;
}

// Checks 'just pressed'-effect timers and sets the redraw flag
void bar_check_timers(bar_t *bar) {

  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);

  bar_entry_t *current = bar->first_entry;
  while (current) {
    double diff = now.tv_sec - current->last_pressed.tv_sec +
                  (now.tv_nsec - current->last_pressed.tv_nsec) / 1000000000.0;

    bool just_pressed_new = (diff <= bar->click_effect_duration);
    if (current->just_pressed != just_pressed_new) {
      bar->needs_redraw = true;
      current->just_pressed = just_pressed_new;
    }
    current = current->next;
  }
}

void bar_line(bar_t *bar) {
  printf(",[");

  int i = 0;
  bool needs_comma = false;
  bar_entry_t *current = bar->first_entry;
  while (current) {
    if (current->call_predraw)
      current->call_predraw(bar, current);

    if (current->icon) {
      if (needs_comma)
        printf(",");
      printf("{");

      if (current->just_pressed)
        printf("\"urgent\": true,");

      printf("\"name\": \"%i\","
             "\"full_text\": \"  %s  \","
             "\"separator\": false,"
             "\"min_width\": \"  " ICON_MIN_WIDTH "  \","
             "\"separator_block_width\": 0"
             "}",
             i, current->icon);

      needs_comma = true;
    }

    if (current->text) {
      if (needs_comma)
        printf(",");
      printf("{");
      printf("\"name\": \"%i\",", i);
      printf("\"full_text\": \"");
      wm_ipc_json_print_escaped(current->text);
      printf("\",");
      printf("\"separator\": false,");
      printf("\"separator_block_width\": 20");
      printf("}");
      needs_comma = true;
    }

    current = current->next;
    i++;
  }

  printf("]\n");

  bar->needs_redraw = false;
}

void bar_update(bar_t *bar) {
  bar_stdin_parse(bar);
  bar_check_timers(bar);
  if (bar->needs_redraw)
    bar_line(bar);
}

void bar_cleanup(bar_t *bar) {
  bar_entry_t *current = bar->first_entry;
  while (current) {
    bar_entry_t *old = current;
    current = old->next;
    free(old);
  }

  free(bar);
}
