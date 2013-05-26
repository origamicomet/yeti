// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/render/d3d11/pixel_shader.h>

#include <butane/application.h>
#include <butane/render/device.h>
#include <butane/render/d3d11/device.h>
#include <butane/render/d3d11/shader_compiler.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("pixel shaders", Allocators::heap());
    return allocator;
  }

  D3D11PixelShader::D3D11PixelShader(
    const Resource::Id id
  ) : butane::PixelShader(id)
    , _resource(nullptr)
  {
  }

  D3D11PixelShader::~D3D11PixelShader()
  {
    if (_resource)
      _resource->Release();
  }

  PixelShader* PixelShader::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    D3D11PixelShader* ps = make_new(D3D11PixelShader, allocator())(id);
    D3D11RenderDevice* rd = (D3D11RenderDevice*)Application::render_device();
    const HRESULT hr = rd->device()->CreatePixelShader(
      stream.memory_resident_data(), stream.memory_resident_data_len(), NULL, &ps->_resource);
    if (FAILED(hr))
      fail("ID3D11Device::CreatePixelShader failed, hr=%#08x", hr);
    return ps;
  }

  void PixelShader::unload(
    PixelShader* ps )
  {
    assert(ps != nullptr);
    make_delete(D3D11PixelShader, allocator(), (D3D11PixelShader*)ps);
  }

  bool PixelShader::compile(
    const Resource::Compiler::Source& src,
    const Resource::Compiler::Stream& cs )
  {
    static const D3D_SHADER_MACRO defines[] = {
      { "PIXEL_SHADER", "1" },
      { "D3D11",        "1" },
      { NULL, NULL },
    };

  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
    static const UINT flags =
      D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY |
      D3DCOMPILE_OPTIMIZATION_LEVEL0 |
      D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
  #else
    static const UINT flags =
      D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY |
      D3DCOMPILE_OPTIMIZATION_LEVEL3 |
      D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
  #endif

    size_t shader_len = 0;
    void* shader = File::read_in(cs.source_data(), Allocators::heap(), &shader_len);

    if (!shader)
      return false;

    ID3DBlob* blob = nullptr;
    ID3DBlob* err_blob = nullptr;

    D3DInclude include;
    include.src = src;

    const HRESULT hr = D3DCompile(
      shader, shader_len, src.path,
      &defines[0], &include,
      "ps_main", "ps_4_0",
      flags, 0, &blob, &err_blob);

    Allocators::heap().free(shader);

    if (FAILED(hr)) {
      log("D3DCompile failed, hr=%#08x", hr);
      log("  %s", err_blob->GetBufferPointer());
      err_blob->Release();
      return false;
    }

    if (!File::write_out(cs.memory_resident_data(), (const void*)blob->GetBufferPointer(), blob->GetBufferSize())) {
      blob->Release();
      return false; }

    blob->Release();
    return true;
  }
} // butane
