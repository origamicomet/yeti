// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/sampler.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("samplers", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  D3D11Sampler::D3D11Sampler()
    : _interface(nullptr)
  {
  }

  D3D11Sampler::~D3D11Sampler()
  {
    if (_interface)
      _interface->Release();
    _interface = nullptr;
  }

  static D3D11_TEXTURE_ADDRESS_MODE d3d11_texture_address_from_butane(
    const Sampler::TextureAddressingMode::Value addressing_mode )
  {
    switch (addressing_mode) {
      case Sampler::TextureAddressingMode::WRAP:
        return D3D11_TEXTURE_ADDRESS_WRAP;
      case Sampler::TextureAddressingMode::MIRROR:
        return D3D11_TEXTURE_ADDRESS_MIRROR;
      case Sampler::TextureAddressingMode::CLAMP:
        return D3D11_TEXTURE_ADDRESS_CLAMP;
      case Sampler::TextureAddressingMode::BORDER:
        return D3D11_TEXTURE_ADDRESS_BORDER;
      case Sampler::TextureAddressingMode::MIRROR_ONCE:
        return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
      default:
        __builtin_unreachable();
    }
  }

  static D3D11_FILTER d3d11_filter_from_butane(
    const Sampler::Filter::Value filter )
  {
    switch (filter) {
      case Sampler::Filter::MIN_MAG_MIP_POINT:
        return D3D11_FILTER_MIN_MAG_MIP_POINT;
      case Sampler::Filter::MIN_MAG_POINT_MIP_LINEAR:
        return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
      case Sampler::Filter::MIN_POINT_MAG_LINEAR_MIP_POINT:
        return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
      case Sampler::Filter::MIN_POINT_MAG_MIP_LINEAR:
        return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
      case Sampler::Filter::MIN_LINEAR_MAG_MIP_POINT:
        return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
      case Sampler::Filter::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
        return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
      case Sampler::Filter::MIN_MAG_LINEAR_MIP_POINT:
        return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
      case Sampler::Filter::MIN_MAG_MIP_LINEAR:
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      case Sampler::Filter::ANISOTROPIC:
        return D3D11_FILTER_ANISOTROPIC;
      default:
        __builtin_unreachable();
    }
  }

  Sampler* Sampler::create(
    const Desc& desc )
  {
    D3D11Sampler* sampler =
      make_new(D3D11Sampler, allocator())();

    D3D11_SAMPLER_DESC ssd;
    ssd.Filter = d3d11_filter_from_butane(desc.filter);
    ssd.AddressU = d3d11_texture_address_from_butane(desc.uvw[0]);
    ssd.AddressV = d3d11_texture_address_from_butane(desc.uvw[1]);
    ssd.AddressW = d3d11_texture_address_from_butane(desc.uvw[2]);
    ssd.MinLOD = -FLT_MAX;
    ssd.MaxLOD = FLT_MAX;
    ssd.MipLODBias = 0.0f;
    ssd.MaxAnisotropy = 16;
    ssd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    ssd.BorderColor[0] = 0.0f;
    ssd.BorderColor[1] = 0.0f;
    ssd.BorderColor[2] = 0.0f;
    ssd.BorderColor[3] = 0.0f;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* sampler->_interface = */ {
      const HRESULT hr = render_device->device()->CreateSamplerState(
        &ssd, &sampler->_interface);
      if (FAILED(hr))
        fail("ID3D11Device::CreateSamplerState failed, hr=%#08x", hr);
    }

    return sampler;
  }

  void D3D11Sampler::destroy()
  {
    make_delete(D3D11Sampler, allocator(), this);
  }
} // butane

