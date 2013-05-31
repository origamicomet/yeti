// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/texture.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("textures", Allocators::heap());
    return allocator;
  }

  D3D11Texture::D3D11Texture(
    const Type type,
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height,
    const uint32_t depth
  ) : Texture(type, pixel_format, width, height, depth)
    , _resource(nullptr)
    , _srv(nullptr)
  {
  }

  D3D11Texture::~D3D11Texture()
  {
    if (_resource)
      _resource->Release();
    if (_srv)
      _resource->Release();
  }

  Texture* Texture::create(
    const Type type,
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height,
    const uint32_t depth,
    const uint32_t flags,
    const void* data )
  {
    D3D11Texture* texture = make_new(D3D11Texture, allocator())(
      type, pixel_format, width, height, depth);

    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem = data;
    srd.SysMemPitch = pixel_format.row_stride(width);
    srd.SysMemSlicePitch = pixel_format.layer_stride(width, height);

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    switch (type) {
      case Texture::TEXTURE_1D: {
        if (height != 1)
          fail("Unexpected height (%u) for one dimensional texture!", height);

        D3D11_TEXTURE1D_DESC td;
        td.Width          = width;
        td.MipLevels      = 0;
        td.ArraySize      = depth;
        td.Format         = dxgi_format_from_pixel_format(pixel_format);
        td.Usage          = D3D11_USAGE_IMMUTABLE;
        td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0;
        td.MiscFlags      = 0;

        /* texture->_resource = */ {
          const HRESULT hr = render_device->device()->CreateTexture1D(
            &td, &srd, (ID3D11Texture1D**)&texture->_resource);
          if (FAILED(hr))
            fail("ID3D11Device::CreateTexture1D failed, hr=%#08x", hr);
        }

        srvd.Format = td.Format;
        srvd.ViewDimension = (depth > 1) ?
          D3D11_SRV_DIMENSION_TEXTURE1DARRAY :
          D3D11_SRV_DIMENSION_TEXTURE1D;
        srvd.Texture1DArray.MostDetailedMip = 0;
        srvd.Texture1DArray.MipLevels = -1;
        srvd.Texture1DArray.FirstArraySlice = 0;
        srvd.Texture1DArray.ArraySize = depth;
      } break;

      case Texture::TEXTURE_2D: {
        D3D11_TEXTURE2D_DESC td;
        td.Width              = width;
        td.Height             = height;
        td.MipLevels          = 1;
        td.ArraySize          = depth;
        td.Format             = dxgi_format_from_pixel_format(pixel_format);
        td.SampleDesc.Count   = 1;
        td.SampleDesc.Quality = 0;
        td.Usage              = D3D11_USAGE_IMMUTABLE;
        td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags     = 0;
        td.MiscFlags          = 0;

        /* texture->_resource = */ {
          const HRESULT hr = render_device->device()->CreateTexture2D(
            &td, &srd, (ID3D11Texture2D**)&texture->_resource);
          if (FAILED(hr))
            fail("ID3D11Device::CreateTexture2D failed, hr=%#08x", hr);
        }

        srvd.Format = td.Format;
        srvd.ViewDimension = (depth > 1) ?
          D3D11_SRV_DIMENSION_TEXTURE2DARRAY :
          D3D11_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2DArray.MostDetailedMip = 0;
        srvd.Texture2DArray.MipLevels = -1;
        srvd.Texture2DArray.FirstArraySlice = 0;
      } break;

      case Texture::TEXTURE_3D: {
        D3D11_TEXTURE3D_DESC td;
        td.Width          = width;
        td.Height         = height;
        td.Depth          = depth;
        td.MipLevels      = 0;
        td.Format         = dxgi_format_from_pixel_format(pixel_format);
        td.Usage          = D3D11_USAGE_IMMUTABLE;
        td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0;
        td.MiscFlags      = 0;

        /* texture->_resource = */ {
          const HRESULT hr = render_device->device()->CreateTexture3D(
            &td, &srd, (ID3D11Texture3D**)&texture->_resource);
          if (FAILED(hr))
            fail("ID3D11Device::CreateTexture3D failed, hr=%#08x", hr);
        }

        srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
        srvd.Texture3D.MostDetailedMip = 0;
        srvd.Texture3D.MipLevels = -1;
      } break;

      case Texture::CUBE_MAP: {
        if (depth != 6)
          fail("Unexpected depth (%u) for a cube map!", depth);

        D3D11_TEXTURE2D_DESC td;
        td.Width          = width;
        td.Height         = height;
        td.MipLevels      = 0;
        td.ArraySize      = depth;
        td.Format         = dxgi_format_from_pixel_format(pixel_format);
        td.Usage          = D3D11_USAGE_IMMUTABLE;
        td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0;
        td.MiscFlags      = 0;

        /* texture->_resource = */ {
          const HRESULT hr = render_device->device()->CreateTexture2D(
            &td, &srd, (ID3D11Texture2D**)&texture->_resource);
          if (FAILED(hr))
            fail("ID3D11Device::CreateTexture2D failed, hr=%#08x", hr);
        }

        srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        srvd.TextureCube.MostDetailedMip = 0;
        srvd.TextureCube.MipLevels = -1;
      } break;
    }

    /* texture->_srv = */ {
      const HRESULT hr = render_device->device()->CreateShaderResourceView(
        texture->_resource, &srvd, &texture->_srv);
      if (FAILED(hr))
        fail("ID3D11Device::CreateShaderResourceView failed, hr=%#08x", hr);
    }

    // D3D11_SAMPLER_DESC sd;
    // sd.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    // sd.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    // sd.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    // sd.MipLODBias     = 0.0f;
    // sd.MaxAnisotropy  = 16;
    // sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    // sd.BorderColor[0] = 0.0f;
    // sd.BorderColor[1] = 0.0f;
    // sd.BorderColor[2] = 0.0f;
    // sd.BorderColor[3] = 0.0f;
    // sd.MinLOD         = -FLT_MAX;
    // sd.MaxLOD         = FLT_MAX;

    // sd.Filter = (texture_blob->flags & HAS_MIP_MAPS) ?
    //   D3D11_FILTER_ANISOTROPIC :
    //   D3D11_FILTER_MIN_MAG_MIP_POINT;

    // /* texture->_ss = */ {
    //   const HRESULT hr = render_device->device()->CreateSamplerState(
    //     &sd, &texture->_ss);
    //   if (FAILED(hr))
    //     fail("ID3D11Device::CreateSamplerState failed, hr=%#08x", hr);
    // }

    return texture;
  }

  void D3D11Texture::destroy()
  {
    make_delete(D3D11Texture, allocator(), this);
  }
} // butane
