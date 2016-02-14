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
#include <stdbool.h>

#define I3_SOCKET_CMD "i3 --get-socketpath"
#define I3_SOCKET_BUFFER_LEN 100

#define WM_IPC_MSG_COMMAND 0
#define WM_IPC_MSG_WORKSPACES 1
#define WM_IPC_MSG_SUBSCRIBE 2
#define WM_IPC_MSG_OUTPUTS 3
#define WM_IPC_MSG_TREE 4
#define WM_IPC_MSG_MARKS 5
#define WM_IPC_MSG_BAR_CONFIG 6
#define WM_IPC_MSG_VERSION 7

/*
        --------------------------------------------------------------------
        Lowlevel Functions
        --------------------------------------------------------------------
*/

// returns the socket file descriptor (needs to be close()'d!)
int wm_ipc_connect();

/*
        Send a message to an already connected WM IPC socket.
        See also: http://i3wm.org/docs/ipc.html#_sending_messages_to_i3

        Parameters:
                sock:
                        Return value of wm_ipc_connect().

                message_type:
                        One of the WM_IPC_MSG_* defines from above.

                json_payload:
                        NULL or an actual JSON string.

*/
void wm_ipc_send(int sock, uint32_t message_type, char *json_payload);

/*
        Handle any incoming messages from the already connected WM IPC
        socket with a callback function.

        Parameters:
                sock:
                        Return value of wm_ipc_connect().

                callback(bool is_event_reply, uint32_t message_type,
                char* payload, void* userdata):
                        Your callback function, that will be called for every
                        message, that has arrived. json_payload is always zero-
                        terminated. See also:
                        http://i3wm.org/docs/ipc.html#_receiving_replies_from_i3

                userdata:
                        Any data, that you may use in your callback function.

                blocking:
                        When set to true, this function waits until at least one
                        message has arrived. Otherwise, it directly returns when
                        there is no message.
*/
void wm_ipc_recv_handle(int sock,
                        void (*callback)(bool is_event_reply,
                                         uint32_t message_type,
                                         char *json_payload, void *userdata),
                        void *userdata, bool blocking);

/*
        --------------------------------------------------------------------
        Lazy JSON parser (just enough to process WM messages)

        Parameters
                json_payload:
                        Is always assumed to be a null-terminated string.
                key:
                        Must be in quotation marks, eg: "name".
        --------------------------------------------------------------------
*/

/*
        Return value:
                NULL (no string found)
                new buffer with found string (needs to be free'd!)
*/
char *wm_ipc_json_get_string(char *json_payload, char *key);

/*
        Return value:
                0: false
                1: true
                -1: not found
*/
char wm_ipc_json_get_bool(char *json_payload, char *key);

// print a string to stdout, with '"' and '\' escaped
void wm_ipc_json_print_escaped(const char *string);
