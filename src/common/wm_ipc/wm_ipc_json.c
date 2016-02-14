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

#include "wm_ipc.h"
#include <err.h>
#include <stdlib.h>
#include <string.h>

/*
        Return value:
                NULL (no string found)
                new buffer with found string (needs to be free'd!)
*/
char *wm_ipc_json_get_string(char *json_payload, char *key) {
  char *pos = strstr(json_payload, key);
  if (!pos)
    return NULL;
  pos += strlen(key);

  pos = strstr(pos, "\"");
  if (!pos)
    return NULL;
  char *pos_end = strstr(pos + 1, "\"");
  if (!pos_end)
    return NULL;

  size_t len = pos_end - pos - 1;
  char *ret = malloc(len + 1);
  strncpy(ret, pos + 1, len);
  ret[len] = '\0';

  return ret;
}

/*
        Return value:
                0: false
                1: true
                -1: not found
*/
char wm_ipc_json_get_bool(char *json_payload, char *key) {
  char *pos = strstr(json_payload, key);
  if (!pos)
    return -1;

  pos += strlen(key);

  char *pos_end = strstr(pos, ",");
  if (!pos_end)
    return -1;

  size_t len = pos_end - pos;
  char *str = malloc(len + 1);
  strncpy(str, pos, len);
  str[len] = '\0';

  int ret;
  if (!strcmp(str, "true"))
    ret = 1;
  else if (!strcmp(str, "false"))
    ret = 0;
  else
    ret = -1;
  free(str);
  return ret;
}

void wm_ipc_json_print_escaped(const char *string) {
  char escape_me[] = {'"', '\\',
                      /* this just marks the end: */ '\0'};

  while (*string) {
    if (strchr(escape_me, *string)) {
      putchar('\\');
    }
    putchar(*string);
    string++;
  }
}
