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

#include <lwe/asset_compiler.h>
#include <lwe/asset.h>

#include <stdlib.h>
#include <stdio.h>

static bool _compile(
  lwe_type_id_t type_id,
  lwe_const_str_t data,
  lwe_const_str_t data_src,
  lwe_const_str_t path )
{
  const lwe_asset_type_t* type = lwe_asset_type_id_to_type(type_id);
  if (!type)
    return false;

  lwe_const_str_t rel_path = lwe_path_strip(data_src, path);
  lwe_fail_if(rel_path == NULL, "Unable to strip data_src from path");
  const lwe_hash_t hash = lwe_murmur_hash((rel_path + 1), 0);

  char paths[2][LWE_MAX_PATH];
  sprintf(&paths[0][0], "%s/streams/%" LWE_HASH_FORMAT ".mrd", data, hash);
  sprintf(&paths[1][0], "%s/streams/%" LWE_HASH_FORMAT ".stream", data, hash);

  FILE* in = fopen(path, "rb");
  lwe_fail_if(!in, "Unable to open input file `%s`", path);

  FILE* mrd = fopen(paths[0], "wb");
  lwe_fail_if(!mrd, "Unable to create memory-resident data file");

  FILE* stream = fopen(paths[1], "wb");
  lwe_fail_if(!stream, "Unable to create stream data file");

  lwe_asset_compile_data_t acd = {
    data,
    data_src,
    path, in, mrd, stream
  };

  lwe_log("Compiling `%s`\n\n", path);

  if (!type->compile(type_id, &acd)) {
    lwe_log("  > Did not succesfully compile\n\n");

    fclose(in);
    fclose(mrd);
    fclose(stream);

    remove(paths[0]);
    remove(paths[1]);

    return false;
  }

  lwe_log("  > Succesfully compiled\n\n");

  fclose(in);
  fclose(mrd);
  fclose(stream);

  return true;
}

typedef struct lwe_compile_data_t {
  lwe_const_str_t data;
  lwe_const_str_t data_src;
  lwe_size_t num_assets;
  lwe_size_t num_succesfuly_compiled_assets;
} lwe_compile_data_t;

static void _on_found(
  lwe_compile_data_t* cd,
  lwe_const_str_t path )
{
  const lwe_type_id_t type_id = lwe_asset_path_to_type_id(path);
  if (type_id == LWE_ASSET_TYPE_ID_INVALID)
    return;

  cd->num_assets++;
  if (_compile(type_id, cd->data, cd->data_src, path))
    cd->num_succesfuly_compiled_assets++;
}

void lwe_asset_compiler_compile(
  lwe_const_str_t data,
  lwe_const_str_t data_src,
  lwe_const_str_t path )
{
  lwe_assert(data != NULL);
  lwe_assert(data_src != NULL);
  lwe_assert(path != NULL);

  const lwe_type_id_t type_id = lwe_asset_path_to_type_id(path);
  if (type_id == LWE_ASSET_TYPE_ID_INVALID)
    return;

  {
    char dir[LWE_MAX_PATH];
    sprintf(&dir[0], "%s/streams", data);
    lwe_mkdir(&dir[0]);
  }

  _compile(type_id, data, data_src, path);
}

void lwe_asset_compiler_compile_dir(
  lwe_const_str_t data,
  lwe_const_str_t data_src )
{
  lwe_assert(data != NULL);
  lwe_assert(data_src != NULL);

  lwe_log("\nStarting asset compile...\n\n >  data=`%s`\n >  data_src=`%s`\n\n", data, data_src);

  {
    char dir[LWE_MAX_PATH];
    sprintf(&dir[0], "%s/streams", data);
    lwe_mkdir(&dir[0]);
  }

  lwe_compile_data_t cd = {
    data,
    data_src,
    0, 0
  };

  lwe_fsearch(data_src, (void*)&cd, (lwe_fsearch_callback_t)&_on_found);

  lwe_log(
    "Compiled %u/%u (%u%%) assets succesfully.\n",

    cd.num_succesfuly_compiled_assets,
    cd.num_assets,
    (cd.num_succesfuly_compiled_assets * 100) / (cd.num_assets ? cd.num_assets : 1)
  );
}