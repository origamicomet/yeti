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

#include <lwe/asset.h>

static lwe_array_t<lwe_asset_type_t> _asset_types;

static lwe_asset_type_t* extension_to_asset_type(
  lwe_const_str_t ext )
{
  for (lwe_size_t i = 0; i < _asset_types.size; ++i) {
    lwe_asset_type_t* type = lwe_array_index(&_asset_types, i);

    if (strcmp(type->assoc_ext, ext) != 0)
      continue;

    return type;
  }

  return NULL;
}

lwe_asset_type_t* lwe_asset_register_type(
  lwe_type_id_t type_id,
  lwe_const_str_t assoc_ext,
  lwe_asset_type_load_t load,
  lwe_asset_type_unload_t unload,
  lwe_asset_type_compile_t compile )
{
  lwe_assert(assoc_ext != NULL);
  lwe_assert(load != NULL);
  lwe_assert(unload != NULL);
  lwe_assert(compile != NULL);

  lwe_fail_if(
    lwe_asset_type_id_to_type(type_id) != NULL,
    "Asset type already registered, type_id=%u\n",
    type_id
  );

  lwe_asset_type_t type = {
    type_id,
    assoc_ext,
    load,
    unload,
    compile
  };

  const lwe_size_t index = lwe_array_push(&_asset_types, &type);
  lwe_log("Registered asset type, type_id=%u assoc_ext=%s\n", type_id, assoc_ext);
  return lwe_array_index(&_asset_types, index);
}

lwe_type_id_t lwe_asset_path_to_type_id(
  lwe_const_str_t path )
{
  lwe_const_str_t ext = lwe_path_find_ext(path);
  lwe_asset_type_t* type = extension_to_asset_type(ext);
  return type ? type->type_id : LWE_ASSET_TYPE_ID_INVALID;
}

lwe_asset_type_t* lwe_asset_to_type(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  return lwe_asset_type_id_to_type(asset->type_id);
}

lwe_asset_type_t* lwe_asset_type_id_to_type(
  lwe_type_id_t type_id )
{
  for (lwe_size_t i = 0; i < _asset_types.size; ++i) {
    lwe_asset_type_t* type = lwe_array_index(&_asset_types, i);

    if (type->type_id != type_id)
      continue;

    return type;
  }

  return NULL;
}

#include <lwe/assets/texture.h>
#include <lwe/assets/vertex_shader.h>
#include <lwe/assets/pixel_shader.h>
#include <lwe/assets/material.h>
#include <lwe/assets/model.h>

void lwe_asset_register_types()
{
  lwe_texture_register_type();
  lwe_vertex_shader_register_type();
  lwe_pixel_shader_register_type();
  lwe_material_register_type();
  lwe_model_register_type();
}