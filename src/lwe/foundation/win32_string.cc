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

#include <lwe/foundation/string.h>
#include <lwe/foundation/platforms/windows.h>

lwe_size_t lwe_to_native_str(
  lwe_const_str_t str,
  int str_len,
  lwe_native_str_t native_str )
{
  const int size = MultiByteToWideChar(
    CP_UTF8, 0, str, str_len, NULL, 0
  );

  if (!native_str)
    return size * sizeof(lwe_native_char_t);

  MultiByteToWideChar(
    CP_UTF8, 0, str, str_len, (LPWSTR)native_str, size
  );

  return size * sizeof(lwe_native_char_t);
}

lwe_size_t lwe_from_native_str(
  lwe_const_native_str_t native_str,
  int native_str_len,
  lwe_str_t str )
{
  const int size = WideCharToMultiByte(
    CP_UTF8, 0, native_str, native_str_len, NULL, 0, NULL, NULL
  );

  if (!str)
    return size;

  WideCharToMultiByte(
    CP_UTF8, 0, native_str, native_str_len, str, size, NULL, NULL
  );

  return size;
}