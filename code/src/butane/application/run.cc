// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/math.h>
#include <butane/script.h>
#include <butane/resource.h>
#include <butane/resources/config.h>
#include <butane/resources/script.h>
#include <butane/render_config.h>
#include <butane/tied_resources.h>
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
#include <butane/time_step_policy.h>

namespace butane {
namespace Application {
  static RenderDevice* _render_device = nullptr;

  RenderDevice* render_device()
  { return _render_device; }

  void set_render_device(
    RenderDevice* render_device )
  { _render_device = render_device; }

  static Resource::Handle<RenderConfig> _render_config;

  Resource::Handle<RenderConfig>& render_config()
  { return _render_config; }

  static void create_or_update_global_resources();
  static void destroy_global_resources();

  void set_render_config(
    Resource::Handle<RenderConfig>& render_config )
  {
    if (_render_config.valid())
      destroy_global_resources();
    _render_config = render_config;
    create_or_update_global_resources();
  }

  static Array<void*>& __globals_initializer() {
    static Array<void*> globals(Allocators::heap());
    return globals; }

  static const thread_safe::Static< Array<void*> >
    __ts_globals(&__globals_initializer);

  Array<void*>& globals()
  { return __ts_globals(); }

  static Array<Window*>& __windows_initializer() {
    static Array<Window*> windows(Allocators::heap());
    return windows; }

  static const thread_safe::Static< Array<Window*> >
    __ts_windows(&__windows_initializer);

  Array<Window*>& windows()
  { return __ts_windows(); }

  static Array<SwapChain*>& __swap_chains_initializer() {
    static Array<SwapChain*> swap_chains(Allocators::heap());
    return swap_chains; }

  static const thread_safe::Static< Array<SwapChain*> >
    __ts_swap_chains(&__swap_chains_initializer);

  Array<SwapChain*>& swap_chains()
  { return __ts_swap_chains(); }

  static Array<TiedResources*>& __tied_resources_initializer() {
    static Array<TiedResources*> tied_resources(Allocators::heap());
    return tied_resources; }

  static const thread_safe::Static< Array<TiedResources*> >
    __ts_tied_resources(&__tied_resources_initializer);

  Array<TiedResources*>& tied_resources()
  { return __ts_tied_resources(); }

  static void on_swap_chain_resized(
    void* /* unused */,
    const SwapChain* swap_chain )
  {
    create_or_update_global_resources();
  }

  static void on_main_window_closed(
    void* /* unused */,
    Window* window )
  {
    Application::quit();
  }

  static void on_window_closed(
    void* /* unused */,
    Window* window )
  {
    destroy_window_and_swap_chain_and_resources(window);
  }

  void create_window_and_swap_chain_and_resources(
    const char* title,
    const uint32_t width,
    const uint32_t height,
    Window*& window,
    SwapChain*& swap_chain,
    TiedResources*& swap_chain_and_resources )
  {
    window = Window::open(title, width, height);
    window->set_on_closed_handler(&on_window_closed);
    Application::windows() += window;
    swap_chain = SwapChain::create(window, PixelFormat::R8G8B8A8, width, height, false, false);
    swap_chain->set_on_resized_handler(&on_swap_chain_resized);
    Application::swap_chains() += swap_chain;
    swap_chain_and_resources = TiedResources::create(swap_chain);
    Application::tied_resources() += swap_chain_and_resources;
  }

  static const Array<Window*>::Iterator find_window( const Window* window ) {
    for (auto iter = Application::windows().begin(); iter != Application::windows().end(); ++iter)
      if ((*iter) == window)
        return iter;
    return Application::windows().end();
  }

  static const Array<SwapChain*>::Iterator find_swap_chain( const Window* window ) {
    for (auto iter = Application::swap_chains().begin(); iter != Application::swap_chains().end(); ++iter)
      if ((*iter)->window() == window)
        return iter;
    return Application::swap_chains().end();
  }

  static const Array<TiedResources*>::Iterator find_swap_chain_and_resources( const SwapChain* swap_chain ) {
    for (auto iter = Application::tied_resources().begin(); iter != Application::tied_resources().end(); ++iter)
      if ((*iter)->swap_chain() == swap_chain)
        return iter;
    return Application::tied_resources().end();
  }

  void destroy_window_and_swap_chain_and_resources(
    Window* window )
  {
    assert(window != nullptr);
    auto window_iter = find_window(window);
    auto swap_chain_iter = find_swap_chain(*window_iter);
    auto swap_chain_and_resources_iter = find_swap_chain_and_resources(*swap_chain_iter);
    (*swap_chain_and_resources_iter)->destroy_global_resources();
    (*swap_chain_and_resources_iter)->destroy();
    Application::tied_resources().remove(swap_chain_and_resources_iter);
    (*swap_chain_iter)->destroy();
    Application::swap_chains().remove(swap_chain_iter);
    (*window_iter)->close();
    Application::windows().remove(window_iter);
  }

  static Array<World*>& __worlds_initializer() {
    static Array<World*> worlds(Allocators::heap());
    return worlds; }

  static const thread_safe::Static< Array<World*> >
    __ts_worlds(&__worlds_initializer);

  Array<World*>& worlds()
  { return __ts_worlds(); }

  static TimeStepPolicy _time_step_policy = TimeStepPolicy::variable();

  TimeStepPolicy& time_step_policy()
  { return _time_step_policy; }

  void set_time_step_policy(
    TimeStepPolicy& time_step_policy )
  {
    _time_step_policy = time_step_policy;
  }

  static void create_or_update_global_resources()
  {
    assert(_render_config.valid());

    uint32_t minimum_width_to_support, minimum_height_to_support; {
      minimum_width_to_support = minimum_height_to_support = 0;
      for (auto iter = swap_chains().begin(); iter != swap_chains().end(); ++iter) {
        minimum_width_to_support = max(minimum_width_to_support, (*iter)->width());
        minimum_height_to_support = max(minimum_height_to_support, (*iter)->height()); }
    }

    globals().resize(_render_config->globals().size());
    for (size_t idx = 0; idx < _render_config->globals().size(); ++idx) {
      const RenderConfig::Resource& resource =
        _render_config->globals()[idx];
      switch (resource.type) {
        case RenderConfig::Resource::RENDER_TARGET:
          if (globals()[idx])
            ((Texture*)globals()[idx])->destroy();
          if (!swap_chains().empty()) {
            globals()[idx] = (void*)Texture::create(
              Texture::TEXTURE_2D,
              resource.render_or_depth_stencil_target.format,
              minimum_width_to_support * resource.render_or_depth_stencil_target.scale.x,
              minimum_height_to_support * resource.render_or_depth_stencil_target.scale.y,
              1, Texture::RENDER_TARGETABLE); }
          break;
        case RenderConfig::Resource::DEPTH_STENCIL_TARGET:
          if (globals()[idx])
            ((Texture*)globals()[idx])->destroy();
          if (!swap_chains().empty()) {
            globals()[idx] = (void*)Texture::create(
              Texture::TEXTURE_2D,
              resource.render_or_depth_stencil_target.format,
              minimum_width_to_support * resource.render_or_depth_stencil_target.scale.x,
              minimum_height_to_support * resource.render_or_depth_stencil_target.scale.y,
              1, Texture::DEPTH_STENCIL_TARGETABLE); }
          break;
        default:
          __builtin_unreachable();
      }
    }

    for (auto iter = tied_resources().begin(); iter != tied_resources().end(); ++iter)
      (*iter)->create_or_update_global_resources();
  }

  static void destroy_global_resources()
  {
    assert(_render_config.valid());

    for (size_t idx = 0; idx < _render_config->globals().size(); ++idx) {
      const RenderConfig::Resource& resource =
        _render_config->globals()[idx];
      switch (resource.type) {
        case RenderConfig::Resource::RENDER_TARGET:
        case RenderConfig::Resource::DEPTH_STENCIL_TARGET:
          if (globals()[idx])
            ((Texture*)globals()[idx])->destroy();
          globals()[idx] = nullptr;
        default:
          __builtin_unreachable();
      }
    }

    for (auto iter = tied_resources().begin(); iter != tied_resources().end(); ++iter)
      (*iter)->destroy_global_resources();
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
      window->set_on_closed_handler(&on_main_window_closed);
      windows() += window;
    }

    RenderDevice* rd; {
      double adapater;
      if (!manifest->find("application.graphics.adapter", adapater))
        adapater = 0;

      rd = RenderDevice::create((uint32_t)adapater);
      Application::set_render_device(rd);
    }

    Resource::Handle<RenderConfig> render_config; {
      const char* config;
      if (!manifest->find("application.graphics.config", config))
        fail("Expected `application.graphics.config` to be specified!");
      const Resource::Id id = Resource::Id(RenderConfig::type(), config);
      render_config = id;
      Application::set_render_config(render_config);
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

      swap_chains() += swap_chain;
      tied_resources() += TiedResources::create(swap_chain);
      create_or_update_global_resources();
    }

    Script script; {
      const char* path;
      if (!manifest->find("application.boot.script", path))
        fail("Expected `application.boot.script` to be specified!");
      const Resource::Id id = Resource::Id(ScriptResource::type(), path);
      Resource::Handle<ScriptResource> resource = id;
      if (!script.load(path, resource->byte_code().raw(), resource->byte_code().size()))
        fail("Failed to load boot script!");
    }

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

    window->show();

    Timer timer;
    while (true) {
      time_step_policy().frame(timer.microseconds() / 1000000.0f);
      timer.reset();
      for (auto iter = windows().begin(); iter != windows().end(); ++iter)
        (*iter)->update();
      for (size_t step = 0; step < time_step_policy().num_of_steps(); ++step)
        world->update(time_step_policy().delta_time_per_step());
      world->render(camera, Viewport(0, 0, swap_chain->height(), swap_chain->width()), tied_resources()[0]);
    }
  }
} // Application
} // butane
