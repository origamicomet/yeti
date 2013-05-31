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

  // void RenderContext::clear(
  //   const Command::Key key,
  //   DepthStencilTarget* depth_stencil_target,
  //   const float to_depth,
  //   const uint32_t to_stencil )
  // {
  //   assert(depth_stencil_target != nullptr);
  //
  //   D3D11RenderDevice::Commands::ClearDepthStencilView cmd;
  //   cmd.view = ((D3D11DepthStencilTarget*)depth_stencil_target)->view();
  //   cmd.depth = to_depth;
  //   cmd.stencil = to_stencil;
  //
  //   command(key, (const void*)&cmd, sizeof(cmd));
  // }

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
