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

#include "../../common/wm_ipc/wm_ipc.h"
#include <stdio.h>
#include <stdlib.h>

// test subjects:
#include "../winlist.c"

int main() {
  printf(" +++++++++++ penguinphone-alttab Testsuite +++++++++++ \n");
  printf("Please run this with valgrind inside i3/sway.\n");

  printf("Running winlist functions...\n");
  int sock = wm_ipc_connect();
  winlist_entry_t *list = winlist_init(sock);
  winlist_entry_t *win = list;
  while (win) {
    printf("%u: class: %s, title: %s\n", win->id, win->class, win->title);

    win = win->next;
  }
  winlist_cleanup(list);
  close(sock);
}
