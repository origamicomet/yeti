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

#include <lwe/foundation/log.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/net.h>

#include <stdio.h>
#include <stdarg.h>

static FILE* _log = NULL;
static bool _net_log = false;
static lwe_socket_t _console_socket;

void lwe_log_to_file( const char* filename )
{
  if (_log) fclose(_log);
  _log = fopen(filename, "wb");
  setvbuf(_log, NULL, _IONBF, 0);
}

void lwe_log_to_network( const char* address, uint16_t port )
{
  lwe_ipv4_address_t remote_addr;

  lwe_fail_if(
    !lwe_net_str_to_address((lwe_const_str_t)address, &remote_addr),
    "Invalid log address `%s`\n",
    address
  );

  if (!lwe_net_connect(&remote_addr, port, &_console_socket)) {
    lwe_log(
      "Unable to connect to remote console at %s:%u\n",
      address, (uint32_t)port
    );

    return;
  }

  _net_log = true;
}

static void lwe_net_log(
  const char* format,
  va_list va )
{
  char buf[256] = { 0, };
  vsprintf(&buf[0], format, va);
  
  uint32_t msg_len = strlen(buf);

  if (!lwe_net_send(_console_socket, (const void*)&msg_len, sizeof(uint32_t)))
    goto remote_conn_lost;

  while (msg_len > 0) {
    const lwe_size_t sent = lwe_net_send(
      _console_socket, (const void*)&buf[0], msg_len
    );

    if (sent == 0)
      goto remote_conn_lost;

    msg_len -= sent;
  }

  return;

remote_conn_lost:
  _net_log = false;
  lwe_net_close(_console_socket);
}

void lwe_log( const char* format, ... )
{
  va_list va;
  va_start(va, format);
  vfprintf(stdout, format, va);
  if (_log) vfprintf(_log, format, va);
  if (_net_log) lwe_net_log(format, va);
  va_end(va);
}