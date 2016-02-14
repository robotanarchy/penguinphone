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
#include <time.h>

// When adding debug output, make sure to write to stderr:
// 		fprintf(stderr, "look, a stderr line!\n");

int main(int argc, char *argv[]) {
  bar_t *bar = bar_configured_init();
  struct timespec sleep_time = {0, 1E8}; // 0.1s

  while (1) {
    bar_configured_update(bar);
    nanosleep(&sleep_time, NULL);
  }

  // dead code
  bar_configured_cleanup(bar);
  return 0;
}
