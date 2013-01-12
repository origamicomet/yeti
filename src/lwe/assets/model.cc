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

#include <lwe/assets/model.h>
#include <lwe/asset_manager.h>

typedef struct lwe_model_blob_t {
  uint32_t num_meshes;
} lwe_model_blob_t;

typedef struct lwe_mesh_blob_t {
  lwe_hash_t material;
  lwe_vertex_declaration_t vertex_decl;
  lwe_size_t num_indicies;
  lwe_size_t num_vertices;
} lwe_mesh_blob_t;

static lwe_asset_t* lwe_model_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);

  uint8_t* blob = (uint8_t*)lwe_asset_stream_mrd(stream);
  lwe_model_blob_t* model_blob = (lwe_model_blob_t*)(blob + 0);

  lwe_model_t* model =
    (lwe_model_t*)lwe_alloc(
      sizeof(lwe_model_t) + (model_blob->num_meshes - 1) * sizeof(lwe_mesh_t)
    );

  model->num_meshes = model_blob->num_meshes;

  lwe_mesh_blob_t* mesh_blob =
    (lwe_mesh_blob_t*)(blob + sizeof(lwe_model_blob_t));

  for (lwe_size_t i = 0; i < model_blob->num_meshes; ++i) {
    const lwe_size_t vertex_size =
      lwe_vertex_declaration_to_size(mesh_blob->vertex_decl);

    const lwe_size_t num_indicie_bytes =
      mesh_blob->num_indicies * sizeof(uint32_t);

    const lwe_size_t num_vertice_bytes =
      mesh_blob->num_vertices * vertex_size;

    uint8_t* data = ((uint8_t*)mesh_blob) + sizeof(lwe_mesh_blob_t);
    void* indices = (void*)(data);
    void* vertices = (void*)(data + num_indicie_bytes);

    lwe_mesh_t* mesh = &model->meshes[i];

    mesh->material =
      (lwe_material_t*)lwe_asset_manager_find_and_ref_by_hash(
        mesh_blob->material);

    if (!mesh->material) {
      mesh->material =
        (lwe_material_t*)lwe_asset_manager_load_by_hash(
          LWE_ASSET_TYPE_ID_MATERIAL,
          mesh_blob->material
        );
    }

    mesh->vertex_decl = mesh_blob->vertex_decl;
    mesh->num_indicies = mesh_blob->num_indicies;
    mesh->num_vertices = mesh_blob->num_vertices;

    mesh->indicies =
      lwe_index_buffer_create(
        LWE_BUFFER_FLAGS_IMMUTABLE,
        num_indicie_bytes, indices
      );

    mesh->vertices =
      lwe_vertex_buffer_create(
        LWE_BUFFER_FLAGS_IMMUTABLE,
        num_vertice_bytes, vertices
      );

    mesh_blob = (lwe_mesh_blob_t*)(data + num_vertice_bytes + num_indicie_bytes);
  }

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)model;
}

static void lwe_model_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_MODEL);

  lwe_model_t* model = (lwe_model_t*)asset;

  for (lwe_size_t i = 0; i < model->num_meshes; ++i) {
    lwe_mesh_t* mesh = &model->meshes[i];
    lwe_asset_manager_deref(mesh->material);
    lwe_index_buffer_destroy(mesh->indicies);
    lwe_vertex_buffer_destroy(mesh->vertices);
  }

  lwe_free((void*)model);
}

static bool lwe_model_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_MODEL);
  lwe_assert(acd != NULL);

  lwe_log("  > Model compilation not yet supported.\n");
  return FALSE;
}

void lwe_model_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_MODEL,
    "model",
    &lwe_model_load,
    &lwe_model_unload,
    &lwe_model_compile
  );
}