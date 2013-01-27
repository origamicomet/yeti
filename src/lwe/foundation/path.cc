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

#include <lwe/foundation/path.h>

lwe_const_str_t lwe_path_find_ext(
  lwe_const_str_t path )
{
  if (!path)
    return NULL;

  char* ext =
    (char*)(((char*)path) + strlen(path));

  while ((*(--ext) != '.') && (ext != path));

  if (ext == path)
    return NULL;

  return (ext + 1);
}

lwe_const_str_t lwe_path_find_basename(
  lwe_const_str_t path )
{
  if (!path)
    return NULL;

  char* basename =
    (char*)(((char*)path) + strlen(path));

  while ((*(--basename) != '/') && (basename != path));

  if (basename == path)
    return basename;

  return (basename + 1);
}

lwe_const_str_t lwe_path_strip(
  lwe_const_str_t dir,
  lwe_const_str_t path )
{
  if (!dir || !path)
    return path;

  lwe_const_str_t stripped = strstr(path, dir);

  if (!stripped)
    return NULL;

  return (stripped + strlen(dir));
}