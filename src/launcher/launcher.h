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

/*
        There's support for properly parsing freedesktop.org *.desktop
        files in elementary, see:

        https://docs.enlightenment.org/efl/current/group__Efreet__Utils.html

        In the long term, that *could* be used. However, for our prototype
        system, that builds on top of a normal Debian, we don't want the
        menu to be overcrowded by lots of .desktop entries.

        Also using a simple config file is A LOT faster.
*/

typedef struct launcher_entry_t {
  struct launcher_entry_t *next;
  char *title;
  char *cmd;

} launcher_entry_t;

launcher_entry_t *launcher_init(char *config_file);
void launcher_cleanup(launcher_entry_t *first);
