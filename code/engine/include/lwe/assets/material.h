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

#ifndef _LWE_ASSETS_MATERIAL_H_
#define _LWE_ASSETS_MATERIAL_H_

#include <lwe/asset.h>
#include <lwe/constant_buffer.h>
#include <lwe/assets/texture.h>
#include <lwe/assets/vertex_shader.h>
#include <lwe/assets/pixel_shader.h>

typedef struct lwe_material_t
  : public lwe_asset_t
{
  lwe_hash_t group;
  lwe_vertex_shader_t* vertex_shader;
  lwe_pixel_shader_t* pixel_shader;
  lwe_size_t num_constants_bytes;
  lwe_constant_buffer_t* constants;
  lwe_size_t num_textures;
  lwe_texture_t* textures[1];
} lwe_material_t;

extern void lwe_material_register_type();

#endif // _LWE_ASSETS_MATERIAL_H_