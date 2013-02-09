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

#ifndef _LWE_MESH_RENDERER_H_
#define _LWE_MESH_RENDERER_H_

typedef struct lwe_mesh_renderer_t lwe_mesh_renderer_t;

extern lwe_mesh_renderer_t* lwe_mesh_renderer_create(
  lwe_size_t num_render_targets,
  lwe_render_target_t** render_targets,
  lwe_depth_stencil_target_t depth_stencil_target,
  lwe_const_str_t group );

extern void lwe_mesh_renderer_render(
  lwe_mesh_renderer_t* mesh_renderer,
  lwe_camera_t* camera,
  const lwe_size_t* num_culled_objs,
  const lwe_culled_object_t* culled_objs );

extern void lwe_mesh_renderer_destroy(
  lwe_mesh_renderer_t* mesh_renderer );

#endif // _LWE_MESH_RENDERER_H_