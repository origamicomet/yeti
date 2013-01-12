// This file is part of LWE. See README.md for more details.
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

#include "shaders/globals.inl"

struct VS_INPUT {
  float3 position  : POSITION;
  float2 tex_coord : TEXCOORD0;
  float3 normal    : NORMAL;
  float3 binormal  : BINORMAL;
};

struct VS_OUTPUT {
  float3 position  : POSITION;
  float2 tex_coord : TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT IN )
{
  VS_OUTPUT OUT;
  OUT.position = mul(IN.position, Frame.model_view_projection);
  OUT.tex_coord = IN.tex_coord;
  return OUT;
}