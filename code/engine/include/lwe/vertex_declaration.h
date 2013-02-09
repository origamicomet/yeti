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

#ifndef _LWE_VERTEX_DECLARATION_H_
#define _LWE_VERTEX_DECLARATION_H_

typedef enum lwe_vertex_components_t {
  LWE_VERTEX_COMPONENTS_NONE         = 0x00000000u,
  LWE_VERTEX_COMPONENTS_FORCE_UINT32 = 0xFFFFFFFFu,

  LWE_VERTEX_COMPONENTS_POSITION    = (1 << 0),
  LWE_VERTEX_COMPONENTS_COLOR0      = (1 << 1),
  LWE_VERTEX_COMPONENTS_TEXCOORD0   = (1 << 2),
  LWE_VERTEX_COMPONENTS_TEXCOORD1   = (1 << 3),
  LWE_VERTEX_COMPONENTS_NORMAL      = (1 << 4),
  LWE_VERTEX_COMPONENTS_TANGENT     = (1 << 5),
  LWE_VERTEX_COMPONENTS_BINORMAL    = (1 << 6),
  LWE_VERTEX_COMPONENTS_BONEINDICES = (1 << 7),
  LWE_VERTEX_COMPONENTS_BONEWEIGHTS = (1 << 8)
} lwe_vertex_components_t;

typedef struct lwe_vertex_declaration_t {
  union {
    struct {
      unsigned position    : 1;
      unsigned color0      : 1;
      unsigned texcoord0   : 1;
      unsigned texcoord1   : 1;
      unsigned normal      : 1;
      unsigned tangent     : 1;
      unsigned binormal    : 1;
      unsigned boneindices : 1;
      unsigned boneweights : 1;
    };

    uint32_t components;
  };
} lwe_vertex_declaration_t;

extern lwe_size_t lwe_vertex_declaration_to_size(
  lwe_vertex_declaration_t vertex_decl );

#endif // _LWE_VERTEX_DECLARATION_H_