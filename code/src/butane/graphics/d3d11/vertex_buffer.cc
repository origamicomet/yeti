// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/vertex_buffer.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("vertex buffers", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  D3D11VertexBuffer::D3D11VertexBuffer()
    : _resource(nullptr)
  {
  }

  D3D11VertexBuffer::~D3D11VertexBuffer()
  {
    if (_resource)
      _resource->Release();
    _resource = nullptr;
  }

  VertexBuffer* VertexBuffer::create(
    const void* data,
    size_t data_len,
    bool immutable )
  {
    assert(immutable ? (data != nullptr) : true);
    assert(data_len > 0);

    D3D11VertexBuffer* vertex_buffer =
      make_new(D3D11VertexBuffer, allocator())();

    D3D11_BUFFER_DESC bd;
    bd.ByteWidth           = data_len;
    bd.Usage               = immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
    bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags      = immutable ? 0 : D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags           = 0;
    bd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem          = data;
    srd.SysMemPitch      = 0;
    srd.SysMemSlicePitch = 0;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* vertex_buffer->_resource = */ {
      const HRESULT hr = render_device->device()->CreateBuffer(
        &bd, &srd, &vertex_buffer->_resource);
      if (FAILED(hr))
        fail("ID3D11Device::CreateBuffer failed, hr=%#08x", hr);
    }

    return vertex_buffer;
  }

  void D3D11VertexBuffer::destroy()
  {
    make_delete(D3D11VertexBuffer, allocator(), this);
  }

  void D3D11VertexBuffer::update(
    const void* data,
    const size_t data_len )
  {
    assert(data != nullptr);
    assert(data_len > 0);

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    D3D11_MAPPED_SUBRESOURCE msr; {
      const HRESULT hr = render_device->context()->Map(
        _resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
      if (FAILED(hr))
        fail("ID3D11DeviceContext::Map failed, hr=%#08x", hr);
    }

    copy(msr.pData, data, data_len);

    render_device->context()->Unmap(_resource, 0);
  }
} // butane
