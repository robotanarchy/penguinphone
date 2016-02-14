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

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

// test subjects:
#include "../bar.c"
#include "../bar_config.c"
#include "../bar_config_ipc.c"
#include "../bar_stdin.c"

#define FAKE_STDIN(TEXT)                                                       \
  {                                                                            \
    printf("Fake stdin: " TEXT "\n");                                          \
    char *text = TEXT;                                                         \
    fclose(stdin);                                                             \
    stdin = fmemopen(text, strlen(text), "r");                                 \
  }

void callback_onclick(bar_t *bar, bar_entry_t *entry) {
  printf("callback_onclick executed on entry with text: %s!\n", entry->text);

  (*(int *)(bar->userdata))++;
  printf("click count: %i\n", *(int *)(bar->userdata));
}

void test_bar_functions() {
  printf("Running bar_* functions...\n");
  int click_count = 0;
  bar_t *bar = bar_init(0.4, &click_count);
  printf("Empty bar:\n");
  bar_update(bar);

  printf("With three test entries:\n");
  bar_add_entry(bar)->text = "Text only entry";
  bar_add_entry(bar)->icon = "µ";
  bar_entry_t *entry = bar_add_entry(bar);
  entry->text = "Text and icon entry with callback";
  entry->icon = "*";
  entry->call_onclick = callback_onclick;
  bar_update(bar);

  printf("Simulating clicks...\n");
  FAKE_STDIN("(some garbage stdin)");
  bar_update(bar);
  assert(click_count == 0);

  FAKE_STDIN("{\"name\":\"2\",\"button\":1,\"x\": 13,\"y\": 14},");
  bar_update(bar);
  assert(click_count == 1);

  bar_cleanup(bar);
}

void test_bar_configured_functions() {
  printf("Running bar_configured_* functions...\n");
  bar_t *bar = bar_configured_init();
  bar_configured_update(bar);
  bar_configured_cleanup(bar);
}

int main(int argc, char *argv[]) {
  printf(" +++++++++++ penguinphone-status Testsuite +++++++++++ \n");
  printf("Please run this with valgrind inside i3/sway.\n");

  test_bar_functions();
  test_bar_configured_functions();

  fclose(stdin);
  return 0;
}
