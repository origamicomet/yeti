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

#include <lwe/d3d_shader_compiler.h>

HRESULT D3DInclude::Open(
  D3D_INCLUDE_TYPE include_type,
  LPCSTR path,
  LPCVOID /* parent_data */,
  LPCVOID* out_buffer,
  UINT* out_buffer_len )
{
  // What constitutes as a system directory?
  if (include_type == D3D_INCLUDE_SYSTEM)
    return E_NOTIMPL;

  char include_path[LWE_MAX_PATH];
  sprintf(&include_path[0], "%s/%s", acd->data_src, path);

  FILE* fh = fopen(&include_path[0], "rb");
  lwe_fail_if(!fh, "Unable to open `%s`", &path[0]);

  fseek(fh, 0, SEEK_END);
  *out_buffer_len = ftell(fh);
  fseek(fh, 0, SEEK_SET);

  *out_buffer = (LPCVOID)lwe_alloc(*out_buffer_len);

  lwe_fail_if(
    fread((void*)*out_buffer, 1, *out_buffer_len, fh) != *out_buffer_len,
    "Unable to include `%s`, unexpected end-of-file",
    &include_path[0]
  );

  return S_OK;
}

HRESULT D3DInclude::Close(
  LPCVOID buffer )
{
  lwe_assert(buffer != NULL);
  lwe_free((void*)buffer);
  return S_OK;
}