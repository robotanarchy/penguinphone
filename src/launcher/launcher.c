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
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_LEN 300
launcher_entry_t *launcher_init(char *config_file) {
  // open the config
  FILE *file = fopen(config_file, "r");
  if (!file) {
    err(1, "Couldn't open launcher config file: '%s'!\n", config_file);
  }

  // line-by-line parsing
  launcher_entry_t *first = NULL;
  launcher_entry_t *last = NULL;
  char buffer[LINE_BUFFER_LEN + 1];
  while (fgets(buffer, LINE_BUFFER_LEN, file)) {
    // skip comments and empty lines
    if (buffer[0] == '#' || buffer[0] == '\n')
      continue;

    // copy the line to a new buffer
    size_t line_length = strlen(buffer);
    char *copy = malloc(sizeof(char) * (line_length + 1));
    strncpy(copy, buffer, line_length + 1);

    if (!first || last->cmd) {
      // add a new entry and set the title (remove '\n')
      launcher_entry_t *new = calloc(1, sizeof(launcher_entry_t));
      if (first)
        last->next = new;
      else
        first = new;
      last = new;
      last->title = copy;
      copy[line_length - 1] = '\0';
    } else if (last->title) {
      // set the command (replace '\n' with '&')
      last->cmd = copy;
      copy[line_length - 1] = '&';
    }
  }

  // finish up (with sanity checks)
  if (!last)
    err(1, "Your config file is empty!\n");
  if (!last->cmd)
    err(1, "Count of title and cmd strings don't match up!\n");
  fclose(file);
  return first;
}
#undef LINE_BUFFER_LEN

void launcher_cleanup(launcher_entry_t *first) {
  while (first) {
    launcher_entry_t *old = first;
    first = old->next;
    free(old->title);
    free(old->cmd);
    free(old);
  }
}
