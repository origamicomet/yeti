// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/rasterizer_state.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("rasterizer states", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  D3D11RasterizerState::D3D11RasterizerState()
    : _interface(nullptr)
  {
  }

  D3D11RasterizerState::~D3D11RasterizerState()
  {
    if (_interface)
      _interface->Release();
    _interface = nullptr;
  }

  RasterizerState* RasterizerState::create(
    const Desc& desc )
  {
    D3D11RasterizerState* rasterizer_state =
      make_new(D3D11RasterizerState, allocator())();

    D3D11_RASTERIZER_DESC rd;

    switch (desc.fill) {
      case RasterizerState::Fill::SOLID:
        rd.FillMode = D3D11_FILL_SOLID;
        break;
      case RasterizerState::Fill::WIREFRAME:
        rd.FillMode = D3D11_FILL_WIREFRAME;
        break;
      default:
        __builtin_unreachable();
    }

    switch (desc.cull) {
      case RasterizerState::Cull::NONE:
        rd.CullMode = D3D11_CULL_NONE;
        break;
      case RasterizerState::Cull::FRONT:
        rd.CullMode = D3D11_CULL_FRONT;
        break;
      case RasterizerState::Cull::BACK:
        rd.CullMode = D3D11_CULL_BACK;
        break;
      default:
        __builtin_unreachable();
    }

    rd.FrontCounterClockwise = FALSE;
    rd.DepthBias             = 0;
    rd.SlopeScaledDepthBias  = 0.0f;
    rd.DepthBiasClamp        = 0.0f;
    rd.DepthClipEnable       = TRUE;
    rd.ScissorEnable         = desc.scissor;
    rd.MultisampleEnable     = FALSE;
    rd.AntialiasedLineEnable = FALSE;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* rasterizer_state->_interface = */ {
      const HRESULT hr = render_device->device()->CreateRasterizerState(
        &rd, &rasterizer_state->_interface);
      if (FAILED(hr))
        fail("ID3D11Device::CreateRasterizerState failed, hr=%#08x", hr);
    }

    return rasterizer_state;
  }

  void D3D11RasterizerState::destroy()
  {
    make_delete(D3D11RasterizerState, allocator(), this);
  }
} // butane

