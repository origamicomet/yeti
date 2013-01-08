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

#ifndef _LWE_ASSETS_MODEL_H_
#define _LWE_ASSETS_MODEL_H_

#include <lwe/asset.h>
#include <lwe/vertex_declaration.h>
#include <lwe/index_buffer.h>
#include <lwe/vertex_buffer.h>
#include <lwe/assets/material.h>

typedef struct lwe_mesh_t {
  lwe_material_t* material;
  lwe_vertex_declaration_t vertex_decl;
  lwe_size_t num_indicies;
  lwe_size_t num_vertices;
  lwe_index_buffer_t* indicies;
  lwe_vertex_buffer_t* vertices;
} lwe_mesh_t;

typedef struct lwe_model_t
  : public lwe_asset_t
{
  uint32_t num_meshes;
  lwe_mesh_t meshes[1];
} lwe_model_t;

extern void lwe_model_register_type();

#endif // _LWE_ASSETS_MODEL_H_