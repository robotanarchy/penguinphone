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
#include "bar_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bar_config_ipc_callback(bool is_event_reply, uint32_t message_type,
                             char *payload, void *userdata) {
  bar_t *bar = (bar_t *)userdata;
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;
  bar_entry_t *active_win = ud->active_window_title;

  if (message_type == WM_IPC_MSG_SUBSCRIBE) {
    char *change = wm_ipc_json_get_string(payload, "\"change\":");
    char *name = wm_ipc_json_get_string(payload, "\"name\":");
    bool focused = (wm_ipc_json_get_bool(payload, "\"focused\":") == 1);

    if (!strcmp(change, "new"))
      ud->open_window_count++;
    else if (!strcmp(change, "close"))
      ud->open_window_count--;

    if (focused && (!strcmp(change, "new") || !strcmp(change, "title") ||
                    !strcmp(change, "focus"))) {
      if (active_win->text)
        free((void *)active_win->text);

      active_win->text = name;
      if (strlen(name) > CONFIG_ACTIVE_WIN_TITLE_MAXLEN)
        strcpy(name + CONFIG_ACTIVE_WIN_TITLE_MAXLEN - 2, "…");
    } else
      free(name);

    if (ud->open_window_count == 0) {
      ud->window_status = NO_WINDOW_ACTIVE;
      if (active_win->text)
        free((void *)active_win->text);
      active_win->text = NULL;
    } else {
      if (active_win->text && !strcmp(active_win->text, CONFIG_TITLE_ALTTAB))
        ud->window_status = ALTTAB_ACTIVE;
      else
        ud->window_status = OTHER_WINDOW_ACTIVE;
    }

    free(change);
    bar->needs_redraw = true;
  }
}
