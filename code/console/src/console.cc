// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <lwe/foundation.h>

static void usage()
{
  fprintf(stderr,
"usage: console [address]\n"
"\n"
"If address is unspecified it defaults to 127.0.0.1:6000\n"
  );

  exit(EXIT_FAILURE);
}

static lwe_ipv4_address_t local_addr;
static uint16_t local_port;

void parse_args(
  lwe_size_t num_args,
  lwe_const_str_t* args )
{
  const bool valid_addr = (sscanf(
    &args[num_args - 1][0],
    "%uuh.%uuh.%uuh.%uuh:%uh",
    &local_addr.a, &local_addr.b, &local_addr.c, &local_addr.d,
    &local_port
  ) == 5);

  if (!valid_addr) {
    local_addr.a = 127;
    local_addr.b = 0;
    local_addr.c = 0;
    local_addr.d = 1;
    local_port = 6000;
  }
}

#if defined(_MSC_VER)
  #include <conio.h>
#else
  #error Platform and/or compiler not supported.
#endif

int main( lwe_size_t argc, lwe_const_str_t argv[] )
{
  lwe_net_init();
  parse_args(argc, &argv[0]);

  lwe_socket_t listen_socket;
  if (!lwe_net_listen(&local_addr, local_port, &listen_socket)) {
    fprintf(
      stderr,
      "Unable to listen on %uhh.%uhh.%uhh.%uhh:%uh",
      local_addr.a, local_addr.b, local_addr.c, local_addr.d,
      local_port
    );

    return EXIT_FAILURE;
  }

  lwe_socket_t remote_socket;
  if (!lwe_net_accept(listen_socket, &remote_socket, NULL, NULL)) {
    lwe_net_close(listen_socket);
    fprintf(stderr, "Unable to accept remote connection!\n");
    return EXIT_FAILURE;
  }

  while (true) {
    uint32_t msg_len;

    if (!lwe_net_recv(remote_socket, (void*)&msg_len, sizeof(uint32_t)))
      goto remote_conn_lost;

    while (msg_len > 0) {
      char buf[255] = { 0, };

      const lwe_size_t received = lwe_net_recv(
        remote_socket, (void*)&buf[0], (msg_len > 255) ? 255 : msg_len
      );

      if (received == 0)
        goto remote_conn_lost;

      fprintf(stdout, &buf[0]);

      msg_len -= received;
    }

  #if defined(_MSC_VER)
    if (_kbhit()) {
      char buf[255] = { 0, };
      fgets(&buf[0], 255, stdin);
      msg_len = strlen(&buf[0]);

      if (!lwe_net_send(remote_socket, (const void*)&msg_len, sizeof(uint32_t)))
        goto remote_conn_lost;

      while (msg_len > 0) {
        const lwe_size_t sent = lwe_net_send(
          remote_socket, (const void*)&buf[0], msg_len
        );

        if (sent == 0)
          goto remote_conn_lost;

        msg_len -= sent;
      }
    } else {
      msg_len = 0;

      if (!lwe_net_send(remote_socket, (const void*)&msg_len, sizeof(uint32_t)))
        goto remote_conn_lost;
    }
  #endif
  }

remote_conn_lost:
  lwe_net_close(remote_socket);
  lwe_net_close(listen_socket);
  return EXIT_SUCCESS;
}