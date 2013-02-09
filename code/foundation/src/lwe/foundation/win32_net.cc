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

#include <lwe/foundation/net.h>
#include <lwe/foundation/platforms/windows.h>

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>
#include <ws2tcpip.h>

bool lwe_net_str_to_address(
  lwe_const_str_t str,
  lwe_ipv4_address_t* address )
{
  lwe_assert(str != NULL);
  lwe_assert(address != NULL);

  uint32_t a, b, c, d;

  if (sscanf(str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
    return false;

  lwe_fail_if(
    (a > 255) || (b > 255) || (c > 255) || (d > 255),
    "Invalid IPv4 address `%s`", str
  );

  address->a = a;
  address->b = b;
  address->c = c;
  address->d = d;

  return true;
}

bool lwe_net_host_name_to_address(
  lwe_const_str_t str,
  lwe_ipv4_address_t* address )
{
  lwe_assert(str != NULL);
  lwe_assert(address != NULL);

  hostent* ent = gethostbyname(str);

  if (!ent)
    return false;

  const in_addr* addr = (in_addr*)*ent->h_addr_list;
  memcpy((void*)address, (const void*)&addr->S_un.S_addr, 4);

  return true;
}

void lwe_net_init()
{
  WSADATA wd;
  
  lwe_fail_if(
    WSAStartup(MAKEWORD(2, 2), &wd) != 0,
    "Unable to initialize WinSock2"
  );
}

static sockaddr_in _lwe_address_and_port_to_sockaddr(
  const lwe_ipv4_address_t* address,
  uint16_t port )
{
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  memcpy((void*)&addr.sin_addr, (const void*)address, 4);
  return addr;
}

bool lwe_net_connect(
  const lwe_ipv4_address_t* remote_addr,
  uint16_t remote_port,
  lwe_socket_t* socket_ )
{
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (s == INVALID_SOCKET)
    return false;

  const sockaddr_in addr =
    _lwe_address_and_port_to_sockaddr(remote_addr, remote_port);

  if (connect(s, (const sockaddr*)&addr, sizeof(sockaddr_in)) != 0) {
    closesocket(s);
    return false;
  }

  *socket_ = (lwe_socket_t)s;
  return true;
}

bool lwe_net_listen(
  const lwe_ipv4_address_t* local_addr,
  uint16_t local_port,
  lwe_socket_t* socket_ )
{
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (s == INVALID_SOCKET)
    return false;

  const sockaddr_in addr =
    _lwe_address_and_port_to_sockaddr(local_addr, local_port);

  if (bind(s, (const sockaddr*)&addr, sizeof(sockaddr_in)) != 0) {
    closesocket(s);
    return false;
  }

  if (listen(s, SOMAXCONN) != 0) {
    closesocket(s);
    return false;
  }

  *socket_ = (lwe_socket_t)s;
  return true;
}

bool lwe_net_accept(
  lwe_socket_t listen_socket,
  lwe_socket_t* remote_socket,
  lwe_ipv4_address_t* remote_addr,
  uint16_t* remote_port )
{
  sockaddr_in addr;
  int addr_len = sizeof(sockaddr_in);
  SOCKET s = accept(listen_socket, (sockaddr*)&addr, &addr_len);

  if (s == INVALID_SOCKET)
    return false;

  *remote_socket = (lwe_socket_t)s;

  if (remote_addr)
    memcpy((void*)remote_addr, (const void*)&addr.sin_addr, 4);

  if (remote_port)
    *remote_port = ntohs(addr.sin_port);
  
  return true;
}

lwe_size_t lwe_net_recv(
  lwe_socket_t socket,
  void* buffer,
  lwe_size_t num_bytes )
{
  lwe_assert(buffer != NULL);

  int ret = recv((SOCKET)socket, (char*)buffer, num_bytes, 0);
  if (ret <= 0) return 0;
  return (lwe_size_t)ret;
}

lwe_size_t lwe_net_send(
  lwe_socket_t socket,
  const void* buffer,
  lwe_size_t num_bytes )
{
  lwe_assert(buffer != NULL);

  int ret = send((SOCKET)socket, (const char*)buffer, num_bytes, 0);
  if (ret <= 0) return 0;
  return (lwe_size_t)ret;
}  

void lwe_net_close(
  lwe_socket_t socket )
{
  shutdown((SOCKET)socket, SD_BOTH);
  closesocket((SOCKET)socket);
}