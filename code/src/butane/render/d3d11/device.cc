// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/render/d3d11/device.h>

namespace butane {
  D3D11RenderDevice::D3D11RenderDevice()
    : _factory(nullptr)
    , _adapter(nullptr)
    , _device(nullptr)
    , _context(nullptr)
  {
  }

  D3D11RenderDevice::~D3D11RenderDevice()
  {
    if (_factory)
      _factory->Release();
    if (_adapter)
      _adapter->Release();
    if (_device)
      _device->Release();
    if (_context)
      _context->Release();
  }

  void D3D11RenderDevice::dispatch()
  {
  }
} // butane
