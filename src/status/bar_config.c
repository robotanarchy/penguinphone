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

#include "bar_config.h"
#include "../common/wm_ipc/wm_ipc.h"
#include <stdlib.h>
#include <unistd.h>

void callback_close_predraw(bar_t *bar, bar_entry_t *entry) {
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;

  entry->icon =
      (ud->window_status == NO_WINDOW_ACTIVE) ? NULL : CONFIG_ICON_CLOSE;
}

void callback_close_onclick(bar_t *bar, bar_entry_t *entry) {
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;

  wm_ipc_send(ud->wm_socket, WM_IPC_MSG_COMMAND, "kill");
}

void callback_launcher_predraw(bar_t *bar, bar_entry_t *entry) {
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;

  entry->icon = (ud->window_status == OTHER_WINDOW_ACTIVE)
                    ? CONFIG_ICON_ALTTAB
                    : CONFIG_ICON_LAUNCHER;
}

void callback_launcher_onclick(bar_t *bar, bar_entry_t *entry) {
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;

  system((ud->window_status == OTHER_WINDOW_ACTIVE) ? CONFIG_CMD_ALTTAB
                                                    : CONFIG_CMD_LAUNCHER);
}

bar_t *bar_configured_init() {
  // init bar and userdata
  bar_config_userdata_t *ud = calloc(1, sizeof(bar_config_userdata_t));
  bar_t *bar = bar_init(CONFIG_CLICK_EFFECT_DURATION, ud);

  // add entries
  ud->active_window_title = bar_add_entry(bar);

  bar_entry_t *close = bar_add_entry(bar);
  close->call_predraw = callback_close_predraw;
  close->call_onclick = callback_close_onclick;

  bar_entry_t *launcher = bar_add_entry(bar);
  launcher->call_predraw = callback_launcher_predraw;
  launcher->call_onclick = callback_launcher_onclick;

  // Subscribe to the window-event
  // http://i3wm.org/docs/ipc.html#_subscribing_to_events
  ud->wm_socket = wm_ipc_connect();
  wm_ipc_send(ud->wm_socket, WM_IPC_MSG_SUBSCRIBE, "[\"window\"]");

  return bar;
}

void bar_configured_update(bar_t *bar) {
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;

  wm_ipc_recv_handle(ud->wm_socket, bar_config_ipc_callback, bar, false);

  bar_update(bar);
}

void bar_configured_cleanup(bar_t *bar) {
  bar_config_userdata_t *ud = (bar_config_userdata_t *)bar->userdata;

  if (ud->active_window_title->text)
    free((void *)ud->active_window_title->text);
  close(ud->wm_socket);
  free(ud);
  bar_cleanup(bar);
}
