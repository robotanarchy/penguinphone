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

#include "../common/wm_ipc/wm_ipc.h"
#include "bar.h"
#include <err.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
        Find out, which entry belongs to the button_id and call its onclick
        function, if it has any.
*/
void bar_stdin_click(bar_t *bar, int button_id) {
  int i = 0;
  bar_entry_t *current = bar->first_entry;
  while (current) {
    if (i == button_id) {
      if (current->call_onclick) {
        current->call_onclick(bar, current);
        clock_gettime(CLOCK_REALTIME, &current->last_pressed);
      }
      return;
    }

    current = current->next;
    i++;
  }

  err(1, "bar_stdin.c: invalid button id %i!\n", button_id);
}

/*
        Parse stdin line by line to receive notifications about clicked
        buttons. Sway/i3 put a JSON object in each line, look here for more
        info: http://i3wm.org/docs/i3bar-protocol.html
*/
#define LINE_BUFFER_LEN 100
void bar_stdin_parse(bar_t *bar) {
  char buffer[LINE_BUFFER_LEN + 1];
  while (fgets(buffer, LINE_BUFFER_LEN, stdin)) {
    char *number = wm_ipc_json_get_string(buffer, "\"name\"");
    if (number) {
      bar_stdin_click(bar, strtoimax(number, NULL, 10));
      free(number);
    }
  }
}
#undef LINE_BUFFER_LEN
