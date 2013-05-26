// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_SHADER_COMPILER_H_
#define _BUTANE_D3D11_SHADER_COMPILER_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/resource.h>

#define INITGUID
#include <DXGI.h>
#include <D3D11.h>
#include <D3Dcompiler.h>

namespace butane {
  class BUTANE_EXPORT D3DInclude final
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
      Resource::Compiler::Source src;
  };
} // butane

#endif // _BUTANE_D3D11_SHADER_COMPILER_H_
