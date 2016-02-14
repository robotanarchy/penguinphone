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

#include "bar.h"
#include "icons.h"
#include <inttypes.h>

/*
        Right now, all the configuration is done in the bar_config* files.
        This means, you'll need to recompile if you want to change settings.
        For the future, real config files are planned.
*/

/*
        You can pick any icon from "icons.h" here (which, in turn, can have
        all icons from Font Awesome if you put them in there).

        To let it look a bit more like Maemo for example:
        #define CONFIG_ICON_LAUNCHER	ICON_TH
        #define CONFIG_ICON_ALTTAB		ICON_CLONE
*/
#define CONFIG_ICON_LAUNCHER ICON_ROCKET
#define CONFIG_ICON_ALTTAB ICON_BARS
#define CONFIG_ICON_CLOSE ICON_TIMES_CIRCLE

/*
        Maximum length of the active window title, that gets displayed. If
        it is longer, it will get cropped with dots.
*/
#define CONFIG_ACTIVE_WIN_TITLE_MAXLEN 33

/*
        Programs, that get started from the status bar. Make sure to add
        '>&2' at the beginning, so all output gets redirected to stderr.
        Stdout must be exclusively used to communicate with i3bar/swaybar.
*/
#define CONFIG_CMD_LAUNCHER                                                    \
  ">&2 ./bin/penguinphone-launcher ./configs/launcher.cfg &"
#define CONFIG_CMD_ALTTAB ">&2 ./bin/penguinphone-alttab &"

/*
        We just check if the title matches exactly this string - and if it
        does, we assume, that the alttab window is open. Then the launcher
        icon will appear on the top right.
*/
#define CONFIG_TITLE_ALTTAB "alttab"

/*
        After a button gets pressed, it remains in 'urgent' state, so it
        gets an alternative background drawn by the window manager.

        You can set the duration in seconds here, as float.
*/
#define CONFIG_CLICK_EFFECT_DURATION 0.4

typedef enum {
  NO_WINDOW_ACTIVE,
  ALTTAB_ACTIVE,
  OTHER_WINDOW_ACTIVE

} bar_config_window_status_t;

typedef struct {
  bar_config_window_status_t window_status;
  int wm_socket;
  bar_entry_t *active_window_title;
  bar_entry_t *launcher;
  size_t open_window_count;

} bar_config_userdata_t;

// wrappers around functions from bar.c
bar_t *bar_configured_init();
void bar_configured_update(bar_t *bar);
void bar_configured_cleanup(bar_t *bar);

// internal
void bar_config_ipc_callback(bool is_event_reply, uint32_t message_type,
                             char *payload, void *userdata);
