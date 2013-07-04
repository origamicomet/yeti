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
#include <butane/resources/unit.h>
#include <butane/graphics/render_target.h>
#include <butane/graphics/depth_stencil_target.h>
#include <butane/task.h>
#include <butane/console.h>
#include <butane/world.h>

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

  static Array<World*>& __worlds_initializer() {
    static Array<World*> worlds(Allocators::heap());
    return worlds;
  }

  static const thread_safe::Static< Array<World*> >
    __ts_worlds(&__worlds_initializer);

  Array<World*>& worlds() {
    return __ts_worlds();
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
    Task::Scheduler::initialize();

    for (auto iter = args.begin(); iter != args.end(); ++iter) {
      if (strcmp("-log-to-network", *iter) == 0) {
        if ((++iter) == args.end()) {
          log("Malformed command-line argument!");
          log("  Expected: -log-to-network [address/hostname]");
          log("                                    ^ not supplied");
          Application::quit(); }

        Network::Address addr;
        if (!Network::Address::from_host_name((*iter), addr)) {
          if (!Network::Address::from_string((*iter), addr)) {
            warn("Unable to deduce address from -log-to-network %s", (*iter));
            continue; }
        }

        /* TODO: RemoteLogger */
      }
    }

    ConfigResource* manifest =
      (ConfigResource*)Resource::load(ConfigResource::type(), "manifest");

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

    World* world = World::create();
    worlds() += world;

    Unit::Reference camera; {
      const Resource::Id id = Resource::Id(UnitResource::type(), "foundation/units/camera");
      Resource::Handle<UnitResource> type = id;
      Unit& unit = world->unit(world->spawn_unit(type));
      SceneGraph::Node::Camera* camera_ = unit.scene_graph().nodes()[0].camera();
      camera_->set_type(SceneGraph::Node::Camera::ORTHOGRAPHIC);
      camera_->set_near(-1.0f);
      camera_->set_far(1.0f);
      camera_->set_view(Vec2f(-1.0f, -1.0f), Vec2f(1.0f, 1.0f));
      camera = Unit::Reference(unit, 0);
    }

    {
      const Resource::Id id = Resource::Id(UnitResource::type(), "units/splash");
      Resource::Handle<UnitResource> type = id;
      world->spawn_unit(type);
    }

    window->set_on_closed_handler(&on_window_closed);
    window->show();

    while (true) {
      window->update();
      world->update(1.0f / 60.0f);
      // world->render();
    }
  }
} // Application
} // butane
