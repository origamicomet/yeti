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

#ifndef _LWE_ASSET_H_
#define _LWE_ASSET_H_

#include <lwe/foundation.h>
#include <lwe/asset_stream.h>

typedef struct lwe_asset_type_t lwe_asset_type_t;

typedef struct lwe_asset_t {
  lwe_type_id_t       type_id;
  lwe_ref_count_t     ref_count;
  lwe_hash_t          hash;
  lwe_asset_stream_t* stream;
} lwe_asset_t;

typedef lwe_asset_t* (*lwe_asset_type_load_t)(
    lwe_type_id_t type_id,
    lwe_asset_stream_t* stream );

typedef void (*lwe_asset_type_unload_t)(
    lwe_asset_t* asset );

typedef struct lwe_asset_compile_data_t {
  lwe_const_str_t data;
  lwe_const_str_t data_src;
  lwe_const_str_t path;
  FILE *in, *mrd, *stream;
} lwe_asset_compile_data_t;

typedef bool (*lwe_asset_type_compile_t)(
    lwe_type_id_t type_id,
    lwe_asset_compile_data_t* acd );

struct lwe_asset_type_t {
  lwe_type_id_t   type_id;
  lwe_const_str_t assoc_ext;
  lwe_const_str_t assoc_compile_ext;
  lwe_asset_type_load_t load;
  lwe_asset_type_unload_t unload;
  lwe_asset_type_compile_t compile;
};

extern lwe_asset_type_t* lwe_asset_register_type(
  lwe_type_id_t type_id,
  lwe_const_str_t assoc_ext,
  lwe_const_str_t assoc_compile_ext,
  lwe_asset_type_load_t load,
  lwe_asset_type_unload_t unload,
  lwe_asset_type_compile_t compile );

extern lwe_type_id_t lwe_asset_path_to_type_id(
  lwe_const_str_t path );

extern lwe_asset_type_t* lwe_asset_to_type(
  lwe_asset_t* asset );

extern lwe_asset_type_t* lwe_asset_type_id_to_type(
  lwe_type_id_t type_id );

typedef enum lwe_asset_type_id_t {
  LWE_ASSET_TYPE_ID_INVALID       = 0x00000000u,
  LWE_ASSET_TYPE_ID_FORCE_UINT32  = 0xFFFFFFFFu,
  LWE_ASSET_TYPE_ID_TEXTURE       = 0x00000001u,
  LWE_ASSET_TYPE_ID_VERTEX_SHADER = 0x00000002u,
  LWE_ASSET_TYPE_ID_PIXEL_SHADER  = 0x00000003u,
  LWE_ASSET_TYPE_ID_MATERIAL      = 0x00000004u,
  LWE_ASSET_TYPE_ID_MODEL         = 0x00000005u
} lwe_asset_type_id_t;

extern void lwe_asset_register_types();

#endif // _LWE_ASSET_H_