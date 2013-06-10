// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/pixel_shader.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("pixel shaders", Allocators::heap());
    return allocator;
  }

  D3D11PixelShader::D3D11PixelShader(
    const Resource::Id id
  ) : PixelShader(id)
    , _resource(nullptr)
  {
  }

  D3D11PixelShader::~D3D11PixelShader()
  {
    if (_resource)
      _resource->Release();
    _resource = nullptr;
  }

  PixelShader* PixelShader::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("D3D11PixelShader::load");

    const MemoryResidentData& mrd =
      *((const MemoryResidentData*)stream.memory_resident_data());

    D3D11PixelShader* pixel_shader =
      make_new(D3D11PixelShader, allocator())(id);

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    const void* byte_code =
      (const void*)((uintptr_t)stream.memory_resident_data() + sizeof(MemoryResidentData));

    /* pixel_shader->_resource */ {
      const HRESULT hr = render_device->device()->CreatePixelShader(
        byte_code, mrd.byte_code_len,  NULL, &pixel_shader->_resource);

      if (FAILED(hr))
        fail("ID3D11Device::CreatePixelShader failed, hr=%#08x", hr);
    }

    return pixel_shader;
  }

  void PixelShader::unload(
    PixelShader* pixel_shader )
  {
    const LogScope _("D3D11PixelShader::unload");

    assert(pixel_shader != nullptr);
    make_delete(D3D11PixelShader, allocator(), (D3D11PixelShader*)pixel_shader);
  }

  bool PixelShader::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("D3D11PixelShader::compile");

    static const D3D_SHADER_MACRO defines[] = {
      { "PIXEL_SHADER", "1" },
      { "D3D11",        "1" },
      { NULL, NULL },
    };

  #if defined(BUTANE_DEBUG_BUILD) // || defined(BUTANE_DEVELOPMENT_BUILD)
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
    void* shader = File::read_in(input.data, Allocators::heap(), &shader_len);

    if (!shader)
      return false;

    ID3DBlob* blob = nullptr;
    ID3DBlob* err_blob = nullptr;

    D3DInclude include;
    include.input = input;

    const HRESULT hr = D3DCompile(
      shader, shader_len, input.path,
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

    MemoryResidentData mrd;
    mrd.byte_code_len = blob->GetBufferSize();

    if (!File::write_out(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData))) {
      blob->Release();
      return false; }

    if (!File::write_out(output.memory_resident_data, (const void*)blob->GetBufferPointer(), blob->GetBufferSize())) {
      blob->Release();
      return false; }

    blob->Release();
    return true;
  }
} // butane
