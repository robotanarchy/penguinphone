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

#include <stdio.h>
#include <stdlib.h>

// test subjects:
#include "../launcher.c"

int main() {
  printf(" ++++++++++ penguinphone-launcher Testsuite ++++++++++ \n");
  printf("Please run this with valgrind.\n");

  printf("Parsing config example.cfg...\n");
  launcher_entry_t *first = launcher_init("example.cfg");
  launcher_entry_t *current = first;
  while (current) {
    printf("title: %s, cmd: %s\n", current->title, current->cmd);
    current = current->next;
  }
  launcher_cleanup(first);
}
