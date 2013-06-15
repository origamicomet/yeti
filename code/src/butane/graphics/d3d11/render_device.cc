// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_device.h>

#include <butane/graphics/d3d11/render_context.h>
#include <butane/graphics/d3d11/swap_chain.h>
#include <butane/graphics/d3d11/rasterizer_state.h>
#include <butane/graphics/d3d11/depth_stencil_state.h>
#include <butane/graphics/d3d11/blend_state.h>
#include <butane/graphics/d3d11/index_buffer.h>
#include <butane/graphics/d3d11/vertex_buffer.h>
#include <butane/graphics/d3d11/constant_buffer.h>
#include <butane/graphics/d3d11/sampler.h>
#include <butane/graphics/d3d11/texture.h>
#include <butane/graphics/d3d11/vertex_shader.h>
#include <butane/graphics/d3d11/pixel_shader.h>
#include <butane/graphics/d3d11/render_target.h>
#include <butane/graphics/d3d11/depth_stencil_target.h>

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
    , _input_layout_cache(allocator(), 4096)
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
    static const UINT flags = D3D11_CREATE_DEVICE_DEBUG;
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

  void D3D11RenderDevice::dispatch(
    size_t num_render_contexts,
    const RenderContext** render_contexts )
  {
    assert(num_render_contexts > 0);
    assert(render_contexts != nullptr);

    // 1: Gather and Merge
    Array<RenderContext::Command> commands(allocator());
    for (size_t context = 0; context < num_render_contexts; ++context) {
      const size_t offset = commands.size();
      const size_t num_of_cmds = render_contexts[context]->num_of_commands();
      commands.resize(commands.size() + num_of_cmds);
      for (size_t c = 0; c < num_of_cmds; ++c) {
        RenderContext::Command& cmd = commands[offset + c];
        cmd = render_contexts[context]->commands()[c];
        cmd.offset += (uintptr_t)render_contexts[context]->stream(); }
    }

    // 2: Sort
    sort(commands.raw(), commands.size());

    // 3: Build
    for (auto iter = commands.begin(); iter != commands.end(); ++iter)
      dispatch(*((const Command*)(*iter).offset));
  }

  void D3D11RenderDevice::dispatch(
    const Command& command )
  {
    const LogScope _("D3D11RenderDevice::dispatch");

    switch (command.type) {
      case Command::CLEAR_RENDER_TARGET_VIEW: {
        const Commands::ClearRenderTargetView& cmd =
          *((const Commands::ClearRenderTargetView*)&command);
        _context->ClearRenderTargetView(cmd.view, &cmd.rgba[0]);
      } return;

      case Command::CLEAR_DEPTH_STENCIL_VIEW: {
        static const UINT clear_flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
        const Commands::ClearDepthStencilView& cmd =
          *((const Commands::ClearDepthStencilView*)&command);
        _context->ClearDepthStencilView(cmd.view, clear_flags, cmd.depth, cmd.stencil);
      } return;

      case Command::BIND_RENDER_AND_DEPTH_STENCIL_VIEWS: {
        const Commands::BindRenderAndDepthStencilViews& cmd =
          *((const Commands::BindRenderAndDepthStencilViews*)&command);
        _context->OMSetRenderTargets(
          cmd.num_of_render_target_views,
          &cmd.render_target_views[0],
          cmd.depth_stencil_view);
      } return;

      case Command::SET_VIEWPORTS: {
        const Commands::SetViewports& cmd =
          *((const Commands::SetViewports*)&command);
        _context->RSSetViewports(cmd.num_of_viewports, &cmd.viewports[0]);
      } return;

      case Command::DRAW: {
        const Commands::Draw& cmd =
          *((const Commands::Draw*)&command);
        const UINT offsets[1] = { 0, };
        _context->RSSetState(cmd.rasterizer_state);
        _context->OMSetBlendState(cmd.blend_state, NULL, 0xFFFFFFFFu);
        _context->OMSetDepthStencilState(cmd.depth_stencil_state, /* TODO */ 0);
        _context->VSSetShader(cmd.vertex_shader, NULL, 0);
        _context->PSSetShader(cmd.pixel_shader, NULL, 0);
        _context->VSSetSamplers(0, cmd.num_of_samplers_and_textures, &cmd.samplers[0]);
        _context->VSSetShaderResources(0, cmd.num_of_samplers_and_textures, &cmd.textures[0]);
        _context->PSSetSamplers(0, cmd.num_of_samplers_and_textures, &cmd.samplers[0]);
        _context->PSSetShaderResources(0, cmd.num_of_samplers_and_textures, &cmd.textures[0]);
        _context->IASetInputLayout(cmd.input_layout);
        _context->IASetPrimitiveTopology(cmd.topology);
        _context->IASetIndexBuffer(cmd.index_buffer, DXGI_FORMAT_R32_UINT, 0);
        _context->IASetVertexBuffers(0, 1, &cmd.vertex_buffer, &cmd.stride, &offsets[0]);
        _context->VSSetConstantBuffers(0, cmd.num_of_constant_buffers, &cmd.constant_buffers[0]);
        _context->PSSetConstantBuffers(0, cmd.num_of_constant_buffers, &cmd.constant_buffers[0]);
        _context->DrawIndexed(cmd.num_of_indicies, 0, 0);
      } return;

      case Command::PRESENT: {
        const Commands::Present& cmd =
          *((const Commands::Present*)&command);
        cmd.swap_chain->Present(cmd.interval, 0);
      } return;
    }

    __builtin_unreachable();
  }

  ID3D11InputLayout* D3D11RenderDevice::find_or_create_input_layout(
    const VertexDeclaration input,
    const VertexShader* shader )
  {
    const LogScope _("D3D11RenderDevice::find_or_create_input_layout");

    static const D3D11_INPUT_ELEMENT_DESC input_element_descriptors[] = {
      { "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        5, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        6, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",        7, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     1, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     2, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     3, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     4, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     5, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     6, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD",     7, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TANGENT",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "BINORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "BLENDWEIGHT",  0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    if (shader->vertex_declaration().components() & (~input.components()))
      fail("Unable to find or create input layout; shader requires vertex components not supplied by vertex data!");

    const uint64_t hash = (((uint64_t)input.components() << 32) | ((uint64_t)shader->vertex_declaration().components()));
    ID3D11InputLayout* input_layout;
    if (_input_layout_cache.find(hash, input_layout))
      return input_layout;

    UINT offset = 0;
    UINT num_of_input_elements = 0;
    UINT component_offsets[32] = { 0, };
    D3D11_INPUT_ELEMENT_DESC input_elements[32];

    /* 1: Determine the offset per-component based on input. */ {
      static const size_t cs[32] = {
        12,
        16, 16, 16, 16, 16, 16, 16, 16,
        12, 12, 12, 12, 12, 12, 12, 12,
        12, 12, 12,
        4, 4,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      uint32_t components = input.components();
      for (; components; ++num_of_input_elements) {
        const uint32_t component = __builtin_ctz(components);
        component_offsets[component] = offset;
        offset += cs[component];
        components &= (components - 1);
      }
    }

    /* 2: Match the input components with the shader components. */ {
      uint32_t components = shader->vertex_declaration().components();
      for (num_of_input_elements = 0; components; ++num_of_input_elements) {
        const uint32_t component = __builtin_ctz(components);
        input_elements[num_of_input_elements].SemanticName =
          input_element_descriptors[component].SemanticName;
        input_elements[num_of_input_elements].SemanticIndex =
          input_element_descriptors[component].SemanticIndex;
        input_elements[num_of_input_elements].Format =
          input_element_descriptors[component].Format;
        input_elements[num_of_input_elements].InputSlot =
          input_element_descriptors[component].InputSlot;
        input_elements[num_of_input_elements].AlignedByteOffset =
          component_offsets[component];
        input_elements[num_of_input_elements].InputSlotClass =
          input_element_descriptors[component].InputSlotClass;
        input_elements[num_of_input_elements].InstanceDataStepRate =
          input_element_descriptors[component].InstanceDataStepRate;
        components &= (components - 1);
      }
    }

    /* input_layout = */ {
      D3D11VertexShader* shader_ = ((D3D11VertexShader*)shader);
      const HRESULT hr = _device->CreateInputLayout(
        &input_elements[0], num_of_input_elements,
        (const void*)shader_->byte_code().raw(),
        shader_->byte_code().size(),
        &input_layout);
      if (FAILED(hr))
        fail("ID3D11Device::CreateInputLayout failed, hr=%#08x", hr);
    }

    _input_layout_cache.insert(hash, input_layout);
    return input_layout;
  }
} // butane
