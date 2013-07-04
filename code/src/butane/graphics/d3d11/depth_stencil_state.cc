// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/depth_stencil_state.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("depth-stencil states", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  D3D11DepthStencilState::D3D11DepthStencilState()
    : _interface(nullptr)
  {
  }

  D3D11DepthStencilState::~D3D11DepthStencilState()
  {
    if (_interface)
      _interface->Release();
    _interface = nullptr;
  }

  static D3D11_COMPARISON_FUNC d3d11_comparison_func_from_butane(
    const DepthStencilState::Comparison::Value func )
  {
    switch (func) {
      case DepthStencilState::Comparison::NEVER:
        return D3D11_COMPARISON_NEVER;
      case DepthStencilState::Comparison::LESS:
        return D3D11_COMPARISON_LESS;
      case DepthStencilState::Comparison::EQUAL:
        return D3D11_COMPARISON_EQUAL;
      case DepthStencilState::Comparison::LESS_EQUAL:
        return D3D11_COMPARISON_LESS_EQUAL;
      case DepthStencilState::Comparison::GREATER:
        return D3D11_COMPARISON_GREATER;
      case DepthStencilState::Comparison::NOT_EQUAL:
        return D3D11_COMPARISON_NOT_EQUAL;
      case DepthStencilState::Comparison::GREATER_EQUAL:
        return D3D11_COMPARISON_GREATER_EQUAL;
      case DepthStencilState::Comparison::ALWAYS:
        return D3D11_COMPARISON_ALWAYS;
      default:
        __builtin_unreachable();
    }
  }

  static D3D11_STENCIL_OP d3d11_stencil_op_from_butane(
    const DepthStencilState::StencilOp::Value op )
  {
    switch (op) {
      case DepthStencilState::StencilOp::KEEP:
        return D3D11_STENCIL_OP_KEEP;
      case DepthStencilState::StencilOp::ZERO:
        return D3D11_STENCIL_OP_ZERO;
      case DepthStencilState::StencilOp::REPLACE:
        return D3D11_STENCIL_OP_REPLACE;
      case DepthStencilState::StencilOp::INCR_SAT:
        return D3D11_STENCIL_OP_INCR_SAT;
      case DepthStencilState::StencilOp::DECR_SAT:
        return D3D11_STENCIL_OP_DECR_SAT;
      case DepthStencilState::StencilOp::INVERT:
        return D3D11_STENCIL_OP_INVERT;
      case DepthStencilState::StencilOp::INCR:
        return D3D11_STENCIL_OP_INCR;
      case DepthStencilState::StencilOp::DECR:
        return D3D11_STENCIL_OP_DECR;
      default:
        __builtin_unreachable();
    }
  }

  DepthStencilState* DepthStencilState::create(
    const Desc& desc )
  {
    D3D11DepthStencilState* depth_stencil_state =
      make_new(D3D11DepthStencilState, allocator())();

    D3D11_DEPTH_STENCIL_DESC dsd;
    dsd.DepthEnable = desc.tests;
    dsd.DepthWriteMask = desc.writes ?
      D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc = d3d11_comparison_func_from_butane(desc.func);
    dsd.StencilEnable = desc.stencil;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsd.FrontFace.StencilFailOp = d3d11_stencil_op_from_butane(desc.front.fail);
    dsd.FrontFace.StencilDepthFailOp = d3d11_stencil_op_from_butane(desc.front.depth_fail);
    dsd.FrontFace.StencilPassOp = d3d11_stencil_op_from_butane(desc.front.pass);
    dsd.FrontFace.StencilFunc = d3d11_comparison_func_from_butane(desc.front.func);
    dsd.BackFace.StencilFailOp = d3d11_stencil_op_from_butane(desc.back.fail);
    dsd.BackFace.StencilDepthFailOp = d3d11_stencil_op_from_butane(desc.back.depth_fail);
    dsd.BackFace.StencilPassOp = d3d11_stencil_op_from_butane(desc.back.pass);
    dsd.BackFace.StencilFunc = d3d11_comparison_func_from_butane(desc.back.func);

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* depth_stencil_state->_interface = */ {
      const HRESULT hr = render_device->device()->CreateDepthStencilState(
        &dsd, &depth_stencil_state->_interface);
      if (FAILED(hr))
        fail("ID3D11Device::CreateDepthStencilState failed, hr=%#08x", hr);
    }

    return depth_stencil_state;
  }

  void D3D11DepthStencilState::destroy()
  {
    make_delete(D3D11DepthStencilState, allocator(), this);
  }
} // butane

