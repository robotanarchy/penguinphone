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
#include "winlist.h"
#include <Elementary.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  winlist_entry_t *open_windows;
  int sock;

} callback_data_t;

#define CMD_BUFFER_SIZE 60
static void callback_activated(void *data, Evas_Object *obj, void *event_info) {
  callback_data_t *cb_data = (callback_data_t *)data;

  winlist_entry_t *open_windows = cb_data->open_windows;
  const Eina_List *items = elm_list_items_get(obj);

  while (open_windows) {
    if (event_info == eina_list_data_get(items)) {
      elm_exit(); // close GUI

      // send the focus command
      char buffer[CMD_BUFFER_SIZE + 1];
      snprintf(buffer, CMD_BUFFER_SIZE, "[id=\"%u\"] focus", open_windows->id);
      wm_ipc_send(cb_data->sock, WM_IPC_MSG_COMMAND, buffer);

      return;
    }

    open_windows = open_windows->next;
    items = eina_list_next(items);
  }
}
#undef CMD_BUFFER_SIZE

static void callback_unfocused(void *data, Evas_Object *obj, void *event_info) {
  elm_exit();
}

int main(int argc, char **argv) {
  // init window
  if (!elm_init(argc, argv)) {
    err(1, "ELM failed to initialize!\n");
  }
  atexit((void *)elm_shutdown);
  elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
  Evas_Object *win = elm_win_util_standard_add("Main", "alttab");
  elm_win_autodel_set(win, EINA_TRUE);
  evas_object_smart_callback_add(win, "unfocused", callback_unfocused, NULL);

  // add box
  Evas_Object *box = elm_box_add(win);
  evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_win_resize_object_add(win, box);
  evas_object_show(box);

  // add list
  Evas_Object *list = elm_list_add(win);
  evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_box_pack_end(box, list);

  // list callback
  callback_data_t *cb_data = malloc(sizeof(callback_data_t));
  cb_data->sock = wm_ipc_connect();
  cb_data->open_windows = winlist_init(cb_data->sock);
  evas_object_smart_callback_add(list, "activated", callback_activated,
                                 cb_data);

  // fill the list
  winlist_entry_t *current = cb_data->open_windows;
  while (current) {
    elm_list_item_append(list, current->title, NULL, NULL, NULL, NULL);
    current = current->next;
  }

  // show and loop
  evas_object_show(list);
  elm_list_go(list);
  evas_object_show(win);
  elm_run();

  // clean up
  winlist_cleanup(cb_data->open_windows);
  close(cb_data->sock);
  free(cb_data);
  return 0;
}
