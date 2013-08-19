// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_context.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>
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
  void RenderContext::set_render_and_depth_stencil_targets(
    const Command::Key key,
    size_t num_of_render_targets,
    RenderTarget** render_targets,
    DepthStencilTarget* depth_stencil_target )
  {
    assert((num_of_render_targets > 0) ? (render_targets != nullptr) : true);

    D3D11RenderDevice::Commands::BindRenderAndDepthStencilViews cmd;

    D3D11RenderTarget** render_targets_ =
      ((D3D11RenderTarget**)render_targets);

    cmd.num_of_render_target_views = num_of_render_targets;

    for (size_t i = 0; i < num_of_render_targets; ++i) {
      cmd.render_target_views[i] = render_targets_[i]->view();
      cmd.render_target_views[i]->AddRef();
    }

    D3D11DepthStencilTarget* depth_stencil_target_ =
      ((D3D11DepthStencilTarget*)depth_stencil_target);

    cmd.depth_stencil_view =
      depth_stencil_target_ ? depth_stencil_target_->view() : nullptr;

    if (cmd.depth_stencil_view)
      cmd.depth_stencil_view->AddRef();

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::set_viewports(
    const Command::Key key,
    size_t num_of_viewports,
    const Viewport* viewports )
  {
    assert(num_of_viewports > 0);
    assert(num_of_viewports <= 8);
    assert(viewports != nullptr);

    D3D11RenderDevice::Commands::SetViewports cmd;

    cmd.num_of_viewports = num_of_viewports;
    for (size_t i = 0; i < num_of_viewports; ++i) {
      cmd.viewports[i].TopLeftX = viewports[i].left();
      cmd.viewports[i].TopLeftY = viewports[i].top();
      cmd.viewports[i].Width    = viewports[i].width();
      cmd.viewports[i].Height   = viewports[i].height();
      cmd.viewports[i].MinDepth = 0.0f;
      cmd.viewports[i].MaxDepth = 1.0f;
    }

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::clear(
    const Command::Key key,
    RenderTarget* render_target,
    const Vec4f& to_color )
  {
    assert(render_target != nullptr);

    D3D11RenderDevice::Commands::ClearRenderTargetView cmd;
    cmd.view = ((D3D11RenderTarget*)render_target)->view();
    cmd.view->AddRef();
    copy((void*)cmd.rgba, (const void*)&to_color, sizeof(float) * 4);

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::clear(
    const Command::Key key,
    DepthStencilTarget* depth_stencil_target,
    const float to_depth,
    const uint32_t to_stencil )
  {
    assert(depth_stencil_target != nullptr);

    D3D11RenderDevice::Commands::ClearDepthStencilView cmd;
    cmd.view = ((D3D11DepthStencilTarget*)depth_stencil_target)->view();
    cmd.view->AddRef();
    cmd.depth = to_depth;
    cmd.stencil = to_stencil;

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::draw(
    const Command::Key key,
    RasterizerState* rasterizer_state,
    DepthStencilState* depth_stencil_state,
    BlendState* blend_state,
    VertexShader* vertex_shader,
    PixelShader* pixel_shader,
    size_t num_of_samplers_and_textures,
    Sampler** samplers,
    Texture** textures,
    VertexDeclaration vertex_declaration,
    VertexBuffer* verticies,
    IndexBuffer* indicies,
    size_t num_of_constants,
    ConstantBuffer** constants,
    Topology topology,
    size_t num_of_primitives )
  {
    assert(rasterizer_state != nullptr);
    assert(depth_stencil_state != nullptr);
    assert(blend_state != nullptr);
    assert(vertex_shader != nullptr);
    assert(pixel_shader != nullptr);
    assert(num_of_samplers_and_textures <= 8);
    assert((num_of_samplers_and_textures > 0) ? (samplers != nullptr) : true);
    assert((num_of_samplers_and_textures > 0) ? (textures != nullptr) : true);
    assert(vertex_declaration != 0);
    assert(verticies != nullptr);
    assert(indicies != nullptr);
    assert((num_of_constants > 0) ? (constants != nullptr) : true);
    assert(num_of_constants <= 8);
    assert(topology != Topology::INVALID);
    assert(num_of_primitives > 0);

    D3D11RenderDevice::Commands::Draw cmd;
    cmd.rasterizer_state = ((D3D11RasterizerState*)rasterizer_state)->interface();
    cmd.rasterizer_state->AddRef();
    cmd.depth_stencil_state = ((D3D11DepthStencilState*)depth_stencil_state)->interface();
    cmd.depth_stencil_state->AddRef();
    cmd.blend_state = ((D3D11BlendState*)blend_state)->interface();
    cmd.blend_state->AddRef();

    cmd.num_of_samplers_and_textures = num_of_samplers_and_textures;
    D3D11Sampler** samplers_ = ((D3D11Sampler**)samplers);
    D3D11Texture** textures_ = ((D3D11Texture**)textures);
    for (size_t i = 0; i < num_of_samplers_and_textures; ++i) {
      cmd.samplers[i] = samplers_[i]->interface();
      cmd.samplers[i]->AddRef();
      cmd.textures[i] = textures_[i]->srv();
      cmd.textures[i]->AddRef(); }

    cmd.vertex_shader = ((D3D11VertexShader*)vertex_shader)->resource();
    cmd.vertex_shader->AddRef();
    cmd.pixel_shader = ((D3D11PixelShader*)pixel_shader)->resource();
    cmd.pixel_shader->AddRef();

    cmd.stride = vertex_declaration.size();

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    cmd.input_layout = render_device->find_or_create_input_layout(
      vertex_declaration, vertex_shader);
    cmd.input_layout->AddRef();

    cmd.index_buffer = ((D3D11IndexBuffer*)indicies)->resource();
    cmd.index_buffer->AddRef();
    cmd.vertex_buffer = ((D3D11VertexBuffer*)verticies)->resource();
    cmd.vertex_buffer->AddRef();

    cmd.num_of_constant_buffers = num_of_constants;
    D3D11ConstantBuffer** constants_ = ((D3D11ConstantBuffer**)constants);
    for (size_t i = 0; i < num_of_constants; ++i) {
      cmd.constant_buffers[i] = constants_[i]->resource();
      cmd.constant_buffers[i]->AddRef(); }

    switch (topology) {
      case Topology::POINTS:
        cmd.topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        cmd.num_of_indicies = num_of_primitives;
      break;

      case Topology::LINES:
        cmd.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        cmd.num_of_indicies = num_of_primitives / 2;
      break;

      case Topology::LINE_STRIP:
        cmd.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        cmd.num_of_indicies = num_of_primitives - 1;
      break;

      case Topology::TRIANGLES:
        cmd.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        cmd.num_of_indicies = num_of_primitives * 3;
      break;

      case Topology::TRIANGLE_STRIP:
        cmd.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        cmd.num_of_indicies = (num_of_primitives - 3) + 1;
      break;
    }

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::present(
    const Command::Key key,
    SwapChain* swap_chain )
  {
    assert(swap_chain != nullptr);

    D3D11RenderDevice::Commands::Present cmd;
    cmd.swap_chain = ((D3D11SwapChain*)swap_chain)->interface();
    cmd.swap_chain->AddRef();
    cmd.interval = swap_chain->vertical_sync() ? 1 : 0;

    command(key, (const void*)&cmd, sizeof(cmd));
  }
} // butane
