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

#include "launcher.h"
#include <Elementary.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>

static void callback_selected(void *data, Evas_Object *obj, void *event_info) {
  const Eina_List *items = elm_list_items_get(obj);

  launcher_entry_t *current = (launcher_entry_t *)data;
  while (current) {
    if (event_info == eina_list_data_get(items)) {
      elm_exit(); // close GUI
      system(current->cmd);
      return;
    }

    items = eina_list_next(items);
    current = current->next;
  }
}

static void callback_unfocused(void *data, Evas_Object *obj, void *event_info) {
  elm_exit();
}

int main(int argc, char **argv) {
  // parse config
  if (argc != 2)
    err(1, "Syntax: %s path_to_launcher_config", argv[0]);
  launcher_entry_t *config = launcher_init(argv[1]);

  // init window
  if (!elm_init(argc, argv)) {
    err(1, "ELM failed to initialize!\n");
  }
  atexit((void *)elm_shutdown);
  elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
  Evas_Object *win = elm_win_util_standard_add("Main", "launcher");
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

  // add callback
  evas_object_smart_callback_add(list, "selected", callback_selected, config);

  // fill the list
  launcher_entry_t *current = config;
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
  launcher_cleanup(config);
  return 0;
}
