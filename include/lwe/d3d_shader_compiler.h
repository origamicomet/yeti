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

#ifndef _LWE_D3D_SHADER_COMPILER_H_
#define _LWE_D3D_SHADER_COMPILER_H_

#include <lwe/foundation.h>
#include <lwe/foundation/platforms/windows.h>
#include <lwe/asset.h>

#include <stdlib.h>
#include <stdio.h>

#define INITGUID

#include <DXGI.h>
#include <D3D11.h>
#include <D3Dcompiler.h>

class D3DInclude
  : public ID3DInclude
{
  public:
    STDMETHOD(Open)(
      THIS_
      D3D_INCLUDE_TYPE include_types,
      LPCSTR path,
      LPCVOID /* parent_data */,
      LPCVOID *out_buffer,
      UINT *out_buffer_len );

    STDMETHOD(Close)(
      THIS_
      LPCVOID buffer );

  public:
    lwe_asset_compile_data_t* acd;
};

#endif // _LWE_D3D_SHADER_COMPILER_H_