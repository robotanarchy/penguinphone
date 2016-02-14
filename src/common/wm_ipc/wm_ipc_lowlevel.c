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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

// returned value needs to be free'd!
char *wm_ipc_get_socket() {
  // sway
  char *sway_sock = getenv("SWAYSOCK");
  if (sway_sock) {
    size_t len = strlen(sway_sock) + 1;
    char *ret = malloc(len);
    memcpy(ret, sway_sock, len);
    return ret;
  }

  // i3: run the socket command
  FILE *fp = popen(I3_SOCKET_CMD, "r");
  if (fp == NULL)
    err(1, "Failed to get WM socket from SWAYSOCK"
           " environment variable and from the '%s' command!\n",
        I3_SOCKET_CMD);

  char *ret = calloc(1, I3_SOCKET_BUFFER_LEN);
  fgets(ret, I3_SOCKET_BUFFER_LEN - 1, fp);
  pclose(fp);

  // i3: cut off at new line
  char *pos = strstr(ret, "\n");
  if (!pos)
    err(1, "Couldn't parse socket info from '%s'.\n", I3_SOCKET_CMD);
  *pos = '\0';
  return ret;
}

// returns the socket file descriptor (needs to be close()'d!)
int wm_ipc_connect() {
  int ret = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (ret == -1)
    err(1, "Couldn't open WM socket.\n");

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_LOCAL;

  char *socket_path = wm_ipc_get_socket();
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

  if (connect(ret, (const struct sockaddr *)&addr, sizeof(struct sockaddr_un)) <
      0)
    err(1, "Couldn't connect to WM socket: '%s'\n", socket_path);

  free(socket_path);
  return ret;
}

// Unix sockets, fun for the whole family!
bool wm_ipc_got_data(int sock) {
  struct timespec no_sleep = {0, 0};
  fd_set input;
  FD_ZERO(&input);
  FD_SET(sock, &input);

  int ret = pselect(sock + 1, &input, NULL, NULL, &no_sleep, NULL);
  return (ret > 0 && FD_ISSET(sock, &input));
}

#define RECV(BUFFER, LEN)                                                      \
  if (recv(sock, (void *)BUFFER, LEN, 0) == -1)                                \
  err(1, "Receiving %zu byte(s) from wm socket failed.\n", (size_t)LEN)

void wm_ipc_recv_handle(int sock,
                        void (*callback)(bool is_event_reply,
                                         uint32_t message_type,
                                         char *json_payload, void *userdata),
                        void *userdata, bool blocking) {
  if (!blocking && !wm_ipc_got_data(sock))
    return;

  do {
    // verify magic
    char magic[7];
    magic[6] = '\0';
    RECV(magic, 6);
    if (strcmp(magic, "i3-ipc") != 0)
      err(1, "WM socket magic doesn't match (got: %s)!\n", magic);

    // get type and size
    uint32_t message_size, message_type;
    RECV(&message_size, sizeof(uint32_t));
    RECV(&message_type, sizeof(uint32_t));

    // separate the is_event_reply bit
    bool is_event_reply = (message_type & 0x0001);
    message_type &= 0xfffe;

    // store json_payload as zero-terminated string
    char *json_payload = calloc(1, message_size + 1);
    RECV(json_payload, message_size);
    json_payload[message_size] = '\0';

    if (message_type != WM_IPC_MSG_SUBSCRIBE || is_event_reply)
      callback(is_event_reply, message_type, json_payload, userdata);

    free(json_payload);
  } while (wm_ipc_got_data(sock));
}
#undef RECV

#define WRITE(BUFFER, LEN)                                                     \
  if (write(sock, (void *)BUFFER, LEN) == -1)                                  \
  err(1, "Writing %zu byte(s) to wm socket failed.\n", (size_t)LEN)

void wm_ipc_send(int sock, uint32_t message_type, char *json_payload) {
  // won't work without __attribute__((packed)) !
  struct {
    char magic[6];
    uint32_t size;
    uint32_t type;

  } __attribute__((packed))
  header = {{'i', '3', '-', 'i', 'p', 'c'},
            json_payload ? (strlen(json_payload) + 1) : 0,
            message_type};

  WRITE(&header, sizeof(header));
  if (json_payload)
    WRITE(json_payload, header.size);
}
#undef WRITE
