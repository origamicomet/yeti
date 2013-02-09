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

#include <lwe/asset_stream.h>

struct lwe_asset_stream_t {
  lwe_mmap_t* mrd;
  FILE* bfh;
};

lwe_asset_stream_t* lwe_asset_stream_open(
  lwe_const_str_t path )
{
  lwe_assert(path != NULL);
  return lwe_asset_stream_open_by_hash(lwe_murmur_hash(&path[0], 0));
}

lwe_asset_stream_t* lwe_asset_stream_open_by_hash(
  lwe_hash_t hash )
{
  lwe_asset_stream_t* stream =
    (lwe_asset_stream_t*)lwe_alloc(sizeof(lwe_asset_stream_t));

  char path[29] = { 0, };

  sprintf(&path[0], "data/streams/%" LWE_HASH_FORMAT ".mrd", hash);
  stream->mrd = lwe_mmap(&path[0], -1);

  sprintf(&path[0], "data/streams/%" LWE_HASH_FORMAT ".stream", hash);
  stream->bfh = fopen(&path[0], "rb");

  lwe_fail_if(
    (stream->mrd == NULL) && (stream->bfh == NULL),
    "Invalid asset stream (no mrd and no stream), id=%" LWE_HASH_FORMAT,
    hash
  );

  return stream;
}

bool lwe_asset_stream_has_mrd(
  lwe_asset_stream_t* asset_stream )
{
  lwe_assert(asset_stream != NULL);
  return (asset_stream->mrd != NULL);
}

bool lwe_asset_stream_has_stream(
  lwe_asset_stream_t* asset_stream )
{
  lwe_assert(asset_stream != NULL);
  return (asset_stream->bfh != NULL);
}

void* lwe_asset_stream_mrd(
  lwe_asset_stream_t* asset_stream )
{
  lwe_assert(asset_stream != NULL);
  return lwe_mmap_to_ptr(asset_stream->mrd);
}

lwe_file_pos_t lwe_asset_stream_tell(
  lwe_asset_stream_t* asset_stream )
{
  lwe_assert(asset_stream != NULL);

  if (!asset_stream->bfh)
    return -1;

  return ftell(asset_stream->bfh);
}

bool lwe_asset_stream_seek(
  lwe_asset_stream_t* asset_stream,
  lwe_seek_origin_t seek_origin,
  lwe_file_pos_t offset )
{
  lwe_assert(asset_stream != NULL);

  if (!asset_stream->bfh)
    return false;

  switch (seek_origin) {
    case LWE_SEEK_ORIGIN_START:
      return (fseek(asset_stream->bfh, offset, SEEK_SET) != 0);

    case LWE_SEEK_ORIGIN_CURRENT:
      return (fseek(asset_stream->bfh, offset, SEEK_CUR) != 0);

    case LWE_SEEK_ORIGIN_END:
      return (fseek(asset_stream->bfh, offset, SEEK_END) != 0);
  }

  return false;
}

lwe_size_t lwe_asset_stream_read(
  lwe_asset_stream_t* asset_stream,
  lwe_size_t num_bytes,
  void* buffer )
{
  lwe_assert(asset_stream != NULL);
  lwe_assert(buffer != NULL);

  if (!asset_stream->bfh)
    return 0;

  return fread(buffer, 1, num_bytes, asset_stream->bfh);
}

void lwe_asset_stream_close(
  lwe_asset_stream_t* asset_stream )
{
  lwe_assert(asset_stream != NULL);

  if (asset_stream->mrd)
    lwe_mmap_unmap(asset_stream->mrd);

  if (asset_stream->bfh)
    fclose(asset_stream->bfh);

  lwe_free((void*)asset_stream);
}