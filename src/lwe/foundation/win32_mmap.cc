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

#include <lwe/foundation/mmap.h>
#include <lwe/foundation/allocator.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/platforms/windows.h>

struct lwe_mmap_t {
  HANDLE fh;
  HANDLE mh;
  lwe_size_t len;
  void* ptr;
};

lwe_mmap_t* lwe_mmap(
  lwe_const_str_t path,
  int file_len )
{
  HANDLE fh = CreateFile(
    path,
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  if (!fh)
    return NULL;

  HANDLE mh = CreateFileMapping(fh, NULL, PAGE_READONLY, 0, 0, NULL);

  if (!mh) {
    CloseHandle(fh);
    return NULL;
  }

  const SIZE_T map_len = (file_len < 0) ? 0 : file_len;
  void* ptr = MapViewOfFile(mh, FILE_MAP_READ, 0, 0, map_len);

  if (!ptr) {
    CloseHandle(mh);
    CloseHandle(fh);
    return NULL;
  }

  lwe_mmap_t* mmap =
    (lwe_mmap_t*)lwe_alloc(sizeof(lwe_mmap_t));

  mmap->fh = fh;
  mmap->mh = mh;
  mmap->len = (file_len < 0) ? GetFileSize(fh, NULL) : file_len;
  mmap->ptr = ptr;

  return mmap;
}

lwe_size_t lwe_mmap_len(
  lwe_mmap_t* mmap )
{
  lwe_assert(mmap != NULL);
  return mmap->len;
}

void* lwe_mmap_to_ptr(
  lwe_mmap_t* mmap )
{
  lwe_assert(mmap != NULL);
  return mmap->ptr;
}

void lwe_mmap_unmap(
  lwe_mmap_t* mmap )
{
  lwe_assert(mmap != NULL);

  UnmapViewOfFile(mmap->ptr);
  CloseHandle(mmap->mh);
  CloseHandle(mmap->fh);

  lwe_free((void*)mmap);
}