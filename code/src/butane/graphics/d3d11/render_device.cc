// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("render devices", Allocators::heap());
    return allocator;
  }

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

  RenderDevice* RenderDevice::create(
    const uint32_t adapter )
  {
    const LogScope _("D3D11RenderDevice::create");

    D3D11RenderDevice* render_device =
      make_new(D3D11RenderDevice, allocator())();

    /* render_device->_factory = */ {
      const HRESULT hr = CreateDXGIFactory(
        __uuidof(IDXGIFactory), (void**)&render_device->_factory);
      if (FAILED(hr))
        fail("CreateDXGIFactory failed, hr=%#08x", hr);
    }

    /* render_device->_adapter = */ {
      const HRESULT hr = render_device->_factory->EnumAdapters(
        adapter, &render_device->_adapter);
      if (FAILED(hr))
        fail("IDXGIFactory::EnumAdapters failed, hr=%#08x", hr);
    }

  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
    static const UINT flags = 0; // D3D11_CREATE_DEVICE_DEBUG;
  #else
    static const UINT flags = 0;
  #endif

    /* render_device->_device, render_device->_context = */ {
      static const D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
      D3D_FEATURE_LEVEL feature_level_;
      const HRESULT hr = D3D11CreateDevice(
        render_device->_adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, flags, &feature_level, 1, /* D3D11 only */
        D3D11_SDK_VERSION, &render_device->_device, &feature_level_, &render_device->_context);
      if (FAILED(hr))
        fail("D3D11CreateDevice failed, h=%#08x", hr);
    }

    return render_device;
  }

  void D3D11RenderDevice::destroy()
  {
    make_delete(D3D11RenderDevice, allocator(), this);
  }
} // butane
