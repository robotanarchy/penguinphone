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

#include <inttypes.h>

typedef struct winlist_entry_t {
  struct winlist_entry_t *next;
  char *title;
  char *class;
  uint32_t id;

} winlist_entry_t;

winlist_entry_t *winlist_init(int sock);
void winlist_cleanup(winlist_entry_t *list);
