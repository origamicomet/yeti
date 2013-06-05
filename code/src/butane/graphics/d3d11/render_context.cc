// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_context.h>

#include <butane/graphics/d3d11/swap_chain.h>
#include <butane/graphics/d3d11/render_target.h>
#include <butane/graphics/d3d11/depth_stencil_target.h>
#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  void RenderContext::clear(
    const Command::Key key,
    RenderTarget* render_target,
    const Vec4f& to_color )
  {
    assert(render_target != nullptr);

    D3D11RenderDevice::Commands::ClearRenderTargetView cmd;
    cmd.view = ((D3D11RenderTarget*)render_target)->view();
    copy((void*)cmd.rgba, (const void*)&to_color, 16);

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
    cmd.depth = to_depth;
    cmd.stencil = to_stencil;

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::bind_render_and_depth_stencil_views(
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

    for (size_t i = 0; i < num_of_render_targets; ++i)
      cmd.render_target_views[i] = render_targets_[i]->view();

    D3D11DepthStencilTarget* depth_stencil_target_ =
      ((D3D11DepthStencilTarget*)depth_stencil_target);

    cmd.depth_stencil_view =
      depth_stencil_target_ ? depth_stencil_target_->view() : nullptr;

    command(key, (const void*)&cmd, sizeof(cmd));
  }

  void RenderContext::present(
    const Command::Key key,
    SwapChain* swap_chain )
  {
    assert(swap_chain != nullptr);

    D3D11RenderDevice::Commands::Present cmd;
    cmd.swap_chain = ((D3D11SwapChain*)swap_chain)->interface();
    cmd.interval = swap_chain->vertical_sync() ? 1 : 0;

    command(key, (const void*)&cmd, sizeof(cmd));
  }
} // butane
