// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_H_
#define _BUTANE_D3D11_H_

#include <butane/butane.h>
#include <butane/resource.h>
#include <butane/graphics/pixel_format.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define INITGUID
#include <DXGI.h>
#include <D3D11.h>
#include <D3Dcompiler.h>

#undef interface
#undef min
#undef max

namespace butane {
  // Converts a butane::PixelFormat to a DXGI_FORMAT.
  extern BUTANE_EXPORT DXGI_FORMAT dxgi_format_from_pixel_format(
    const PixelFormat pixel_format );

  // See butane/graphics/d3d11/{vertex,pixel,...}_shader.cc for usage.
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
      Resource::Compiler::Input input;
  };
} // butane

#endif // _BUTANE_D3D11_H_
