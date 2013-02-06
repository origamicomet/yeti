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

#include <lwe/assets/material.h>
#include <lwe/asset_manager.h>

#include <libconfig.h>

typedef struct lwe_material_blob_t {
  lwe_hash_t group;
  lwe_hash_t vertex_shader;
  lwe_hash_t pixel_shader;
  lwe_size_t num_constants_bytes;
  lwe_offset_t constants_offset;
  lwe_size_t num_textures;
  lwe_offset_t textures_offset;
} lwe_material_blob_t;

static lwe_asset_t* lwe_material_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_MATERIAL);

  uint8_t* blob = (uint8_t*)lwe_asset_stream_mrd(stream);

  lwe_material_blob_t* material_blob =
    (lwe_material_blob_t*)(blob + 0);

  lwe_hash_t* textures =
    (lwe_hash_t*)(blob + material_blob->textures_offset + sizeof(lwe_material_blob_t));

  void* constants =
    (void*)(blob + material_blob->constants_offset + sizeof(lwe_material_blob_t));

  lwe_material_t* material =
    (lwe_material_t*)lwe_alloc(
      sizeof(lwe_material_t) +
      (material_blob->num_textures - 1) * sizeof(lwe_texture_t*)
    );

  material->group = material_blob->group;

  material->vertex_shader =
    (lwe_vertex_shader_t*)lwe_asset_manager_find_and_ref_by_hash(
      material_blob->vertex_shader);

  if (!material->vertex_shader) {
    material->vertex_shader =
      (lwe_vertex_shader_t*)lwe_asset_manager_load_by_hash(
        LWE_ASSET_TYPE_ID_VERTEX_SHADER,
        material_blob->vertex_shader
      );
  }

  material->pixel_shader =
    (lwe_pixel_shader_t*)lwe_asset_manager_find_and_ref_by_hash(
      material_blob->pixel_shader);

  if (!material->pixel_shader) {
    material->pixel_shader =
      (lwe_pixel_shader_t*)lwe_asset_manager_load_by_hash(
        LWE_ASSET_TYPE_ID_PIXEL_SHADER,
        material_blob->pixel_shader
      );
  }

  material->num_textures = material_blob->num_textures;

  for (lwe_size_t i = 0; i < material->num_textures; ++i) {
    material->textures[i] =
      (lwe_texture_t*)lwe_asset_manager_find_and_ref_by_hash(
        textures[i]);

    if (material->textures[i])
      continue;

    material->textures[i] =
      (lwe_texture_t*)lwe_asset_manager_load_by_hash(
        LWE_ASSET_TYPE_ID_TEXTURE,
        textures[i]
      );

    lwe_fail_if(
      material->textures[i] == NULL,
      "Unable to load associated texture, texture=%" LWE_HASH_FORMAT,
      textures[i]
    );
  }

  if (material_blob->num_constants_bytes > 0) {
    material->constants =
      lwe_constant_buffer_create(
        LWE_BUFFER_FLAGS_IMMUTABLE,
        material_blob->num_constants_bytes,
        constants
      );
  } else {
    material->constants = NULL;
  }

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)material;
}

static void lwe_material_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_MATERIAL);

  lwe_material_t* material = (lwe_material_t*)asset;

  for (lwe_size_t i = 0; i < material->num_textures; ++i)
    lwe_asset_manager_deref((lwe_asset_t*)material->textures[i]);

  lwe_asset_manager_deref((lwe_asset_t*)material->vertex_shader);
  lwe_asset_manager_deref((lwe_asset_t*)material->pixel_shader);

  if (material->constants)
    lwe_constant_buffer_destroy(material->constants);

  lwe_free((void*)material);
}

static bool lwe_material_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_MATERIAL);
  lwe_assert(acd != NULL);

  config_t cfg;
  config_init(&cfg);
  config_set_include_dir(&cfg, acd->data_src);

  if (!config_read(&cfg, acd->in)) {
    config_destroy(&cfg);
    return FALSE;
  }

  lwe_str_t group = NULL;
  if (!config_lookup_string(&cfg, "group", (const char**)&group)) {
    lwe_log("  > Invalid group\n");
    config_destroy(&cfg);
    return FALSE;
  }

  lwe_str_t vertex_shader = NULL;
  if (!config_lookup_string(&cfg, "vertex_shader", (const char**)&vertex_shader)) {
    lwe_log("  > Invalid vertex shader\n");
    config_destroy(&cfg);
    return FALSE;
  }

  lwe_str_t pixel_shader = NULL;
  if (!config_lookup_string(&cfg, "pixel_shader", (const char**)&pixel_shader)) {
    lwe_log("  > Invalid pixel shader\n");
    config_destroy(&cfg);
    return FALSE;
  }

  config_setting_t* textures = config_lookup(&cfg, "textures");
  config_setting_t* constants = config_lookup(&cfg, "constants");

  lwe_material_blob_t blob;
  blob.group = lwe_murmur_hash(group, 0);
  blob.vertex_shader = lwe_murmur_hash(vertex_shader, 0);
  blob.pixel_shader = lwe_murmur_hash(pixel_shader, 0);
  blob.num_constants_bytes = 0;
  blob.constants_offset = 0;
  blob.num_textures = config_setting_length(textures);
  blob.textures_offset = 0;

  if (fwrite((void*)&blob, sizeof(lwe_material_blob_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write to memory-resident-data\n");
    config_destroy(&cfg);
    return FALSE;
  }

  for (lwe_size_t i = 0; i < blob.num_textures; ++i) {
    lwe_const_str_t texture =
      (lwe_const_str_t)config_setting_get_string_elem(textures, i);

    if (!texture) {
      lwe_log("  > Invalid texture, id=%d\n", i);
      config_destroy(&cfg);
      return FALSE;
    }

    const lwe_hash_t hash = lwe_murmur_hash(texture, 0);

    if (fwrite((void*)&hash, sizeof(lwe_hash_t), 1, acd->mrd) != 1) {
      lwe_log("  > Unable to write to memory-resident-data\n");
      config_destroy(&cfg);
      return FALSE;
    }
  }

  config_destroy(&cfg);
  return TRUE;
}

void lwe_material_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_MATERIAL,
    "material", "material",
    &lwe_material_load,
    &lwe_material_unload,
    &lwe_material_compile
  );
}