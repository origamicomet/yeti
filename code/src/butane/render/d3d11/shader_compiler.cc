// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/render/d3d11/shader_compiler.h>

namespace butane {
  HRESULT D3DInclude::Open(
    D3D_INCLUDE_TYPE include_type,
    LPCSTR requested_path,
    LPCVOID /* parent_data */,
    LPCVOID* out_buffer,
    UINT* out_buffer_len )
  {
    // What constitutes as a system directory?
    if (include_type == D3D_INCLUDE_SYSTEM)
      return E_NOTIMPL;

    const String path =
      String::format(Allocators::scratch(), "%s/%s", src.root, requested_path);

    FILE* fh = File::open(path.raw(), "rb");
    if (!fh)
      return E_ACCESSDENIED;

    size_t length = 0;
    *out_buffer = File::read_in(fh, Allocators::heap(), &length);
    *out_buffer_len = length;
    fclose(fh);

    if (!*out_buffer)
      return E_FAIL;

    return S_OK;
  }

  HRESULT D3DInclude::Close(
    LPCVOID buffer )
  {
    assert(buffer != nullptr);
    Allocators::heap().free((void*)buffer);
    return S_OK;
  }
} // butane
