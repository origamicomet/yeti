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

#include <lwe/asset_manager.h>

static lwe_dict_t<lwe_hash_t, lwe_asset_t*> _assets;

lwe_asset_t* lwe_asset_manager_load(
  lwe_const_str_t path )
{
  lwe_assert(path != NULL);

  return lwe_asset_manager_load_by_hash(
    lwe_asset_path_to_type_id(path),
    lwe_murmur_hash(path, 0)
  );
}

lwe_asset_t* lwe_asset_manager_load_by_hash(
  lwe_type_id_t type_id,
  lwe_hash_t hash )
{
  lwe_fail_if(
    lwe_dict_find(&_assets, hash) != NULL,
    "Asset already exits, asset=%" LWE_HASH_FORMAT,
    hash
  );

  lwe_asset_type_t* type = lwe_asset_type_id_to_type(type_id);

  lwe_fail_if(
    type == NULL,
    "Unable to load asset (invalid type), type_id=%u",
    type_id
  );

  lwe_asset_stream_t* asset_stream = lwe_asset_stream_open_by_hash(hash);
  lwe_asset_t* asset = type->load(type_id, asset_stream);
  asset->type_id = type_id;
  asset->ref_count = 1;
  asset->hash = hash;
  asset->stream = asset_stream;
  lwe_dict_insert(&_assets, hash, asset);
  return asset;
}

lwe_asset_t* lwe_asset_manager_find(
  lwe_const_str_t path )
{
  lwe_assert(path != NULL);
  const lwe_hash_t hash = lwe_murmur_hash(path, 0);
  return lwe_asset_manager_find_by_hash(hash);
}

lwe_asset_t* lwe_asset_manager_find_and_ref(
  lwe_const_str_t path )
{
  lwe_assert(path != NULL);
  const lwe_hash_t hash = lwe_murmur_hash(path, 0);
  return lwe_asset_manager_find_and_ref_by_hash(hash);
}

void lwe_asset_manager_deref(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);

  asset->ref_count -= 1;

  if (asset->ref_count > 0)
    return;

  const lwe_hash_t hash = asset->hash;
  lwe_asset_type_t* asset_type = lwe_asset_to_type(asset);
  asset_type->unload(asset);

  lwe_dict_replace(&_assets, hash, (lwe_asset_t*)NULL);
}

lwe_asset_t* lwe_asset_manager_find_by_hash(
  lwe_hash_t hash )
{
  return *lwe_dict_find(&_assets, hash);
}

lwe_asset_t* lwe_asset_manager_find_and_ref_by_hash(
  lwe_hash_t hash )
{
  lwe_asset_t* asset = lwe_asset_manager_find_by_hash(hash);
  if (asset) asset->ref_count += 1;
  return asset;
}