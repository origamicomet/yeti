// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/math.h>
#include <butane/script.h>
#include <butane/script/lua.h>
#include <butane/resource.h>
#include <butane/resources/config.h>
#include <butane/window.h>
#include <butane/graphics/swap_chain.h>
#include <butane/graphics/render_device.h>
#include <butane/graphics/render_context.h>

#include <butane/resources/shader.h>
#include <butane/resources/state.h>
#include <butane/resources/mesh.h>
#include <butane/graphics/render_target.h>
#include <butane/graphics/depth_stencil_target.h>

namespace butane {
namespace Application {
  Array< Pair<uint32_t, Window*> >& windows() {
    static Array< Pair<uint32_t, Window*> > windows(Allocators::heap());
    return windows;
  }

  Array< Pair<uint32_t, SwapChain*> >& swap_chains() {
    static Array< Pair<uint32_t, SwapChain*> > swap_chains(Allocators::heap());
    return swap_chains;
  }

  static void on_window_closed(
    void* closure,
    Window* window )
  {
    window->close();
    Application::quit();
  }

  void run(
    const Array<const char*>& args )
  {
    const LogScope _("Application::run");

    ConfigResource* manifest =
      (ConfigResource*)Resource::load(ConfigResource::type, "manifest");

    manifest =
      (ConfigResource*)Resource::load(ConfigResource::type, "manifest");

    Window* window; {
      String title = String(Allocators::scratch());
      if (!manifest->find("application.window.title", title)) {
        title = String::format(Allocators::scratch(), "butane - %s, %s (%s)",
          Application::build(), Application::platform(), Application::architecture()); }

      Vec2f dims;
      if (!manifest->find("application.window.dimensions", dims))
        dims = Vec2f(1280.0f, 720.0f);

      window = Window::open(title.raw(), (uint32_t)dims.x, (uint32_t)dims.y);
      windows() += Pair<uint32_t, Window*>(0, window);
    }

    RenderDevice* rd; {
      double adapater;
      if (!manifest->find("application.graphics.adapter", adapater))
        adapater = 0;

      rd = RenderDevice::create((uint32_t)adapater);
      Application::set_render_device(rd);
    }

    SwapChain* swap_chain; {
      Vec2f res;
      if (!manifest->find("application.graphics.resolution", res))
        res = Vec2f(1280.0f, 720.0f);

      bool fullscreen;
      if (!manifest->find("application.graphics.fullscreen", fullscreen))
        fullscreen = false;

      bool vertical_sync;
      if (!manifest->find("application.graphics.vertical_sync", vertical_sync))
        vertical_sync = false;

      swap_chain = SwapChain::create(
        window, PixelFormat::R8G8B8A8,
        (uint32_t)res.x, (uint32_t)res.y,
        fullscreen, vertical_sync);

      swap_chains() += Pair<uint32_t, SwapChain*>(0, swap_chain);
    }

    DepthStencilTarget* depth = DepthStencilTarget::create(
      PixelFormat::D24S8, swap_chain->width(), swap_chain->height());

    MeshResource* mesh = (MeshResource*)Resource::load(MeshResource::type, "units/splash/plane");
    ShaderResource* shader = *(mesh->materials()[0].shader);
    StateResource* state = *(shader->state());

    window->set_on_closed_handler(&on_window_closed);
    window->show();

    manifest->dereference();

    RenderContext rc;

    RenderTarget* render_targets[1] = { swap_chain->render_target() };
    rc.set_render_and_depth_stencil_targets(
      0, 1, &render_targets[0], depth);

    const Viewport viewports[1] = {
      Viewport(0, 0, 720, 1280)
    };

    rc.set_viewports(1, 1, &viewports[0]);

    rc.clear(2, swap_chain->render_target(), Vec4f(1.0f, 0.0f, 0.5f, 1.0f));
    rc.clear(3, depth, 1.0f, 0x00000000u);

    rc.draw(
      4, state->rasterizer_state(), state->depth_stencil_state(), state->blend_state(),
      shader->vertex_shader(), shader->pixel_shader(),
      0 /* mesh->materials()[0].num_of_textures */, nullptr, nullptr,
      mesh->vertex_declaration(), mesh->vertices(), mesh->indicies(),
      0, nullptr, Topology::TRIANGLES, mesh->num_of_primitives());

    rc.present(5, swap_chain);

    while (true) {
      window->update();

      const RenderContext* render_contexts[1] = { &rc };
      rd->dispatch(1, &render_contexts[0]);
    }
  }
} // Application
} // butane
