// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/blend_state.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("blend states", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  D3D11BlendState::D3D11BlendState()
    : _interface(nullptr)
  {
  }

  D3D11BlendState::~D3D11BlendState()
  {
    if (_interface)
      _interface->Release();
    _interface = nullptr;
  }

  static D3D11_BLEND d3d11_blend_from_butane(
    const BlendState::Blend::Value op )
  {
    switch (op) {
      case BlendState::Blend::ZERO:
        return D3D11_BLEND_ZERO;
      case BlendState::Blend::ONE:
        return D3D11_BLEND_ONE;
      case BlendState::Blend::SRC_COLOR:
        return D3D11_BLEND_SRC_COLOR;
      case BlendState::Blend::INV_SRC_COLOR:
        return D3D11_BLEND_INV_SRC_COLOR;
      case BlendState::Blend::SRC_ALPHA:
        return D3D11_BLEND_SRC_ALPHA;
      case BlendState::Blend::INV_SRC_ALPHA:
        return D3D11_BLEND_INV_SRC_ALPHA;
      case BlendState::Blend::DEST_ALPHA:
        return D3D11_BLEND_DEST_ALPHA;
      case BlendState::Blend::INV_DEST_ALPHA:
        return D3D11_BLEND_INV_DEST_ALPHA;
      case BlendState::Blend::DEST_COLOR:
        return D3D11_BLEND_DEST_COLOR;
      case BlendState::Blend::INV_DEST_COLOR:
        return D3D11_BLEND_INV_DEST_COLOR;
      case BlendState::Blend::SRC_ALPHA_SAT:
        return D3D11_BLEND_SRC_ALPHA_SAT;
      default:
        __builtin_unreachable();
    }
  }

  static D3D11_BLEND_OP d3d11_blend_op_from_butane(
    const BlendState::BlendOp::Value op )
  {
    switch (op) {
      case BlendState::BlendOp::ADD:
        return D3D11_BLEND_OP_ADD;
      case BlendState::BlendOp::SUBTRACT:
        return D3D11_BLEND_OP_SUBTRACT;
      case BlendState::BlendOp::REV_SUBTRACT:
        return D3D11_BLEND_OP_REV_SUBTRACT;
      case BlendState::BlendOp::MIN:
        return D3D11_BLEND_OP_MIN;
      case BlendState::BlendOp::MAX:
        return D3D11_BLEND_OP_MAX;
      default:
        __builtin_unreachable();
    }
  }

  BlendState* BlendState::create(
    const Desc& desc )
  {
    D3D11BlendState* blend_state =
      make_new(D3D11BlendState, allocator())();

    D3D11_BLEND_DESC bd;
    bd.AlphaToCoverageEnable                 = FALSE;
    bd.IndependentBlendEnable                = FALSE;
    bd.RenderTarget[0].BlendEnable           = desc.enabled;
    bd.RenderTarget[0].SrcBlend              = d3d11_blend_from_butane(desc.color.src);
    bd.RenderTarget[0].DestBlend             = d3d11_blend_from_butane(desc.color.dest);
    bd.RenderTarget[0].BlendOp               = d3d11_blend_op_from_butane(desc.color.op);
    bd.RenderTarget[0].SrcBlendAlpha         = d3d11_blend_from_butane(desc.alpha.src);
    bd.RenderTarget[0].DestBlendAlpha        = d3d11_blend_from_butane(desc.alpha.dest);
    bd.RenderTarget[0].BlendOpAlpha          = d3d11_blend_op_from_butane(desc.alpha.op);
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* blend_state->_interface = */ {
      const HRESULT hr = render_device->device()->CreateBlendState(
        &bd, &blend_state->_interface);
      if (FAILED(hr))
        fail("ID3D11Device::CreateBlendState failed, hr=%#08x", hr);
    }

    return blend_state;
  }

  void D3D11BlendState::destroy()
  {
    make_delete(D3D11BlendState, allocator(), this);
  }
} // butane

