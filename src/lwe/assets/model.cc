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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

typedef struct lwe_model_blob_t {
  uint32_t num_meshes;
} lwe_model_blob_t;

typedef struct lwe_mesh_blob_t {
  mat4_t transform;
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

    memcpy(
      (void*)&mesh->transform,
      (void*)&mesh_blob->transform,
      sizeof(mat4_t)
    );

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

    mesh->input_layout = lwe_input_layout_create(
      mesh->material->vertex_shader, mesh_blob->vertex_decl
    );

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
    lwe_input_layout_destroy(mesh->input_layout);
  }

  lwe_free((void*)model);
}

#include "assimp_stream.inl"

static aiNode* _find_node_by_mesh(
  aiNode* root,
  uint32_t mesh )
{
  for (lwe_size_t child_idx = 0; child_idx < root->mNumChildren; ++child_idx) {
    aiNode* node = _find_node_by_mesh(root->mChildren[child_idx], mesh);
    if (node) return node;
  }

  for (lwe_size_t mesh_idx = 0; mesh_idx < root->mNumMeshes; ++mesh_idx)
    if (root->mMeshes[mesh_idx] == mesh)
      return root;

  return NULL;
}

static bool lwe_model_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_MODEL);
  lwe_assert(acd != NULL);

  IOSystem io_sys;
  io_sys.acd = acd;

  Assimp::Importer importer;
  importer.SetIOHandler(&io_sys);

  const aiScene* scene = importer.ReadFile(
    acd->path,
    aiProcess_CalcTangentSpace |
    aiProcess_CalcTangentSpace |
    aiProcess_JoinIdenticalVertices |
    aiProcess_MakeLeftHanded |
    aiProcess_Triangulate |
    aiProcess_GenNormals |
    aiProcess_LimitBoneWeights |
    aiProcess_ImproveCacheLocality |
    aiProcess_GenUVCoords |
    aiProcess_OptimizeMeshes
  );

  if (!scene) {
    lwe_log("  > Unable to import scene, `%s`\n", importer.GetErrorString());
    return FALSE;
  }

  lwe_log(
    "  > Successfully imported scene, meshes=%u materials=%u textures=%u\n",
    scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures
  );

  lwe_model_blob_t model_blob;
  model_blob.num_meshes = scene->mNumMeshes;

  if (fwrite((void*)&model_blob, sizeof(lwe_model_blob_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write memory-resident data!\n");
    goto failure;
  }

  for (lwe_size_t mesh_idx = 0; mesh_idx < scene->mNumMeshes; ++mesh_idx) {
    aiMesh* mesh = scene->mMeshes[mesh_idx];
    aiNode* node = _find_node_by_mesh(scene->mRootNode, mesh_idx);

    lwe_fail_if(
      (!node) || (node->mName.length == 0),
      "Unable to determine name for mesh, id=%u",
      mesh_idx
    );

    lwe_log(
      "  > Mesh `%s`, id=%u num_vertices=%u num_faces=%u\n",
      node->mName.C_Str(), mesh_idx, mesh->mNumVertices, mesh->mNumFaces
    );

    lwe_mesh_blob_t mesh_blob;

    const aiMatrix4x4& t = node->mTransformation;
    mesh_blob.transform = mat4_t(
      t.a1, t.a2, t.a3, t.a4,
      t.b1, t.b2, t.b3, t.b4,
      t.c1, t.c2, t.c3, t.c4,
      t.d1, t.d2, t.d3, t.d4
    );

    mesh_blob.num_indicies = mesh->mNumFaces * 3;
    mesh_blob.num_vertices = mesh->mNumVertices;
    mesh_blob.vertex_decl.components = 0;

    if (mesh->HasPositions())
      mesh_blob.vertex_decl.position = 1;

    if (mesh->HasNormals())
      mesh_blob.vertex_decl.normal = 1;

    if (mesh->HasTangentsAndBitangents()) {
      mesh_blob.vertex_decl.tangent = 1;
      mesh_blob.vertex_decl.binormal = 1;
    }

    if (mesh->HasVertexColors(0))
      mesh_blob.vertex_decl.color0 = 1;

    if (mesh->GetNumUVChannels() >= 1)
      mesh_blob.vertex_decl.texcoord0 = 1;
    if (mesh->GetNumUVChannels() == 2)
      mesh_blob.vertex_decl.texcoord1 = 1;

    if (mesh->HasBones()) {
      mesh_blob.vertex_decl.boneindices = 1;
      mesh_blob.vertex_decl.boneweights = 1;
    }

    if (!scene->mMaterials) {
      if (mesh->HasBones())
        mesh_blob.material = lwe_murmur_hash("materials/skinned_mesh.material", 0);
      else
        mesh_blob.material = lwe_murmur_hash("materials/static_mesh.material", 0);
    } else {
      char mat_path[LWE_MAX_PATH];
      lwe_const_str_t rel_path = lwe_path_strip(acd->data_src, acd->path) + 1;
      lwe_const_str_t basename = lwe_path_find_basename(rel_path);
      const lwe_size_t len = basename - rel_path;
      strcpy(&mat_path[0], rel_path);
      sprintf(&mat_path[len], "%s.material", node->mName.C_Str());
      mesh_blob.material = lwe_murmur_hash(&mat_path[0], 0);
      lwe_log("  >  material=`%s`\n", &mat_path[0]);
    }

    lwe_log("  >  vertex components:\n");
    lwe_log("  >    POSITION:    [%c]\n", mesh_blob.vertex_decl.position ? 'X' : ' ');
    lwe_log("  >    COLOR0:      [%c]\n", mesh_blob.vertex_decl.color0 ? 'X' : ' ');
    lwe_log("  >    TEXCOORD0:   [%c]\n", mesh_blob.vertex_decl.texcoord0 ? 'X' : ' ');
    lwe_log("  >    TEXCOORD1:   [%c]\n", mesh_blob.vertex_decl.texcoord1 ? 'X' : ' ');
    lwe_log("  >    NORMAL:      [%c]\n", mesh_blob.vertex_decl.normal ? 'X' : ' ');
    lwe_log("  >    TANGENT:     [%c]\n", mesh_blob.vertex_decl.tangent ? 'X' : ' ');
    lwe_log("  >    BINORMAL:    [%c]\n", mesh_blob.vertex_decl.binormal ? 'X' : ' ');
    lwe_log("  >    BONEINDICES: [%c]\n", mesh_blob.vertex_decl.boneindices ? 'X' : ' ');
    lwe_log("  >    BONEWEIGHTS: [%c]\n", mesh_blob.vertex_decl.boneweights ? 'X' : ' ');

    if (fwrite((void*)&mesh_blob, sizeof(lwe_mesh_blob_t), 1, acd->mrd) != 1)
      goto failure;

    for (lwe_size_t face = 0; face < mesh->mNumFaces; ++face) {
      const uint32_t indices[3] = {
        mesh->mFaces[face].mIndices[0],
        mesh->mFaces[face].mIndices[1],
        mesh->mFaces[face].mIndices[2]
      };

      if (fwrite((void*)&indices[0], sizeof(uint32_t), 3, acd->mrd) != 3)
        goto failure;
    }

    for (lwe_size_t vertex = 0; vertex < mesh->mNumVertices; ++vertex) {
      if (mesh_blob.vertex_decl.position)
        if (fwrite((void*)&mesh->mVertices[vertex], sizeof(float), 3, acd->mrd) != 3)
          goto failure;

      if (mesh_blob.vertex_decl.color0)
        if (fwrite((void*)&mesh->mColors[0][vertex], sizeof(float), 4, acd->mrd) != 4)
          goto failure;

      if (mesh_blob.vertex_decl.texcoord0)
        if (fwrite((void*)&mesh->mTextureCoords[0][vertex], sizeof(float), 2, acd->mrd) != 2)
          goto failure;

      if (mesh_blob.vertex_decl.texcoord1)
        if (fwrite((void*)&mesh->mTextureCoords[1][vertex], sizeof(float), 2, acd->mrd) != 2)
          goto failure;

      if (mesh_blob.vertex_decl.normal)
        if (fwrite((void*)&mesh->mNormals[vertex], sizeof(float), 3, acd->mrd) != 3)
          goto failure;

      if (mesh_blob.vertex_decl.tangent)
        if (fwrite((void*)&mesh->mTangents[vertex], sizeof(float), 3, acd->mrd) != 3)
          goto failure;
      
      if (mesh_blob.vertex_decl.binormal)
        if (fwrite((void*)&mesh->mBitangents[vertex], sizeof(float), 3, acd->mrd) != 3)
          goto failure;

      if (mesh_blob.vertex_decl.boneindices || mesh_blob.vertex_decl.boneweights)
        goto failure;
    }
  }

  importer.FreeScene();
  importer.SetIOHandler(NULL);
  return TRUE;

failure:
  lwe_log("  > Unable to write memory-resident data!\n");
  importer.FreeScene();
  importer.SetIOHandler(NULL);
  return FALSE;
}

void lwe_model_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_MODEL,
    "model", "dae",
    &lwe_model_load,
    &lwe_model_unload,
    &lwe_model_compile
  );
}