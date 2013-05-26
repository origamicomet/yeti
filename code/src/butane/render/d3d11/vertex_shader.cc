// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/render/d3d11/vertex_shader.h>

#include <butane/application.h>
#include <butane/render/device.h>
#include <butane/render/d3d11/device.h>
#include <butane/render/d3d11/shader_compiler.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("vertex shaders", Allocators::heap());
    return allocator;
  }

  D3D11VertexShader::D3D11VertexShader(
    const Resource::Id id
  ) : butane::VertexShader(id)
    , _resource(nullptr)
    , _blob(allocator(), (size_t)0)
  {
  }

  D3D11VertexShader::~D3D11VertexShader()
  {
    if (_resource)
      _resource->Release();
  }

  VertexShader* VertexShader::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    D3D11VertexShader* vs = make_new(D3D11VertexShader, allocator())(id);
    uintptr_t mrd = (uintptr_t)stream.memory_resident_data();
    copy((void*)&vs->_vd, (const void*)mrd, sizeof(vs->_vd));
    mrd += sizeof(vs->_vd);
    vs->_blob.resize(stream.memory_resident_data_len() - sizeof(vs->_vd));
    copy((void*)&vs->_blob[0], (const void*)mrd, vs->_blob.size());
    D3D11RenderDevice* rd = (D3D11RenderDevice*)Application::render_device();
    const HRESULT hr = rd->device()->CreateVertexShader(
      (const void*)&vs->_blob[0], vs->_blob.size(), NULL, &vs->_resource);
    if (FAILED(hr))
      fail("ID3D11Device::CreateVertexShader failed, hr=%#08x", hr);
    return vs;
  }

  void VertexShader::unload(
    VertexShader* vs )
  {
    assert(vs != nullptr);
    make_delete(D3D11VertexShader, allocator(), (D3D11VertexShader*)vs);
  }

  bool VertexShader::compile(
    const Resource::Compiler::Source& src,
    const Resource::Compiler::Stream& cs )
  {
    static const D3D_SHADER_MACRO defines[] = {
      { "VERTEX_SHADER", "1" },
      { "D3D11",         "1" },
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

    /* blob, err_blob = */ {
      const HRESULT hr = D3DCompile(
        shader, shader_len, src.path,
        &defines[0], &include,
        "vs_main", "vs_4_0",
        flags, 0, &blob, &err_blob);

      Allocators::heap().free(shader);

      if (FAILED(hr)) {
        log("D3DCompile failed, hr=%#08x", hr);
        log("  %s", err_blob->GetBufferPointer());
        err_blob->Release();
        return false;
      }
    }

    ID3D11ShaderReflection* shader_refl = nullptr; {
      const HRESULT hr = D3DReflect(
        blob->GetBufferPointer(), blob->GetBufferSize(),
        IID_ID3D11ShaderReflection, (void**)&shader_refl);

      if (FAILED(hr)) {
        log("D3DReflect failed, hr=%#08x", hr);
        shader_refl->Release();
        blob->Release();
        return false; }
    }

    VertexDeclaration vd; {
      D3D11_SHADER_DESC desc; {
        const HRESULT hr = shader_refl->GetDesc(&desc);
        if (FAILED(hr)) {
          log("ID3D11ShaderReflection::GetDesc failed, hr=%#08x", hr);
          shader_refl->Release();
          blob->Release();
          return false; }
      }

      for (size_t p = 0; p < desc.InputParameters; ++p) {
        D3D11_SIGNATURE_PARAMETER_DESC param; {
          const HRESULT hr = shader_refl->GetInputParameterDesc(p, &param);
          if (FAILED(hr)) {
            log("ID3D11ShaderReflection::GetInputParamterDesc failed, hr=%#08x", hr);
            shader_refl->Release();
            blob->Release();
            return false; }
        }

        uint32_t component = 0;
        if (strcmp("POSITION", param.SemanticName) == 0)
          component = VertexDeclaration::POSITION;
        else if (strcmp("COLOR", param.SemanticName) == 0)
          component = VertexDeclaration::COLOR0 << param.SemanticIndex;
        else if (strcmp("TEXCOORD", param.SemanticName) == 0)
          component = VertexDeclaration::TEXCOORD0 << param.SemanticIndex;
        else if (strcmp("NORMAL", param.SemanticName) == 0)
          component = VertexDeclaration::NORMAL;
        else if (strcmp("TANGENT", param.SemanticName) == 0)
          component = VertexDeclaration::TANGENT;
        else if (strcmp("BINORMAL", param.SemanticName) == 0)
          component = VertexDeclaration::BINORMAL;
        else if (strcmp("BONEINDICES", param.SemanticName) == 0)
          component = VertexDeclaration::BONEINDICES;
        else if (strcmp("BONEWEIGHTS", param.SemanticName) == 0)
          component = VertexDeclaration::BONEWEIGHTS;
        else {
          log("Unknown vertex component (input semantic) '%s'!", param.SemanticName);
          shader_refl->Release();
          blob->Release();
          return false; }

        if (vd.components() > component) {
          log("Vertex components (input semantics) are out of order.");
          log("  '%s' is out of order.", param.SemanticName);
          shader_refl->Release();
          blob->Release();
          return false; }

        vd.components() |= component;
      }
    }

    shader_refl->Release();

    if (!File::write_out(cs.memory_resident_data(), (const void*)&vd, sizeof(VertexDeclaration))) {
      blob->Release();
      return false; }

    if (!File::write_out(cs.memory_resident_data(), (const void*)blob->GetBufferPointer(), blob->GetBufferSize())) {
      blob->Release();
      return false; }

    blob->Release();
    return true;
  }
} // butane
