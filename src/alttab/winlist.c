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

#include "winlist.h"
#include "../common/wm_ipc/wm_ipc.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
        Parse the output of the get_tree message. i3-compatible WMs list
        like a million nodes, which we don't need here. To see yourself run:

                i3-msg -t get_tree | python -mjson.tool

        As we only need the "window" (window id), "class" and "title"
        fields, simple string parsing will be enough and save resources at
        the same time. The interesting part of the output looks like this:

                ...
                "window": 12345,
                "window_properties":
                {
                        "class": "Roxterm",
                        ...
                        "title": "MYUSER@MYHOST: ~/code/penguinphone",
                        ...
                ...
*/

void winlist_init_callback(bool is_event_reply, uint32_t message_type,
                           char *payload, void *userdata) {
  winlist_entry_t **list = (winlist_entry_t **)userdata;

  winlist_entry_t *last = NULL;
  char *pos = payload;
  while (true) {
    // fast-forward to the next window id
    pos = strstr(pos, "\"window\":");
    if (!pos)
      break;
    pos += strlen("\"window\":");

    char *pos_end = strstr(pos, ",");
    if (!pos_end)
      break;
    *pos_end = '\0';

    uint32_t id = strtoull(pos, NULL, 10);
    pos = pos_end + 1;
    if (!id)
      continue;

    // new window
    winlist_entry_t *win = calloc(1, sizeof(winlist_entry_t));
    win->title = wm_ipc_json_get_string(pos, "\"title\"");
    win->class = wm_ipc_json_get_string(pos, "\"class\"");
    win->id = id;

    // add to list
    if (last)
      last->next = win;
    else
      *list = win;
    last = win;
  }
}

winlist_entry_t *winlist_init(int sock) {
  winlist_entry_t *list = NULL;
  wm_ipc_send(sock, WM_IPC_MSG_TREE, NULL);
  wm_ipc_recv_handle(sock, winlist_init_callback, &list, true);
  return list;
}

void winlist_cleanup(winlist_entry_t *list) {
  while (list) {
    winlist_entry_t *old = list;
    list = list->next;

    if (old->title)
      free(old->title);
    if (old->class)
      free(old->class);

    free(old);
  }
}
