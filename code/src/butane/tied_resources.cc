// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tied_resources.h>

#include <butane/application.h>
#include <butane/render_config.h>
#include <butane/graphics/render_device.h>
#include <butane/graphics/swap_chain.h>
#include <butane/graphics/texture.h>
#include <butane/graphics/render_target.h>
#include <butane/graphics/depth_stencil_target.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("tied resources", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  Allocator& TiedResources::allocator() {
    return __ts_allocator();
  }

  TiedResources::TiedResources(
    SwapChain* swap_chain
  ) : _swap_chain(swap_chain)
    , _globals(allocator())
  {
  }

  TiedResources::~TiedResources()
  {
  }

  TiedResources* TiedResources::create(
    SwapChain* swap_chain )
  {
    assert(swap_chain != nullptr);
    return make_new(TiedResources, allocator())(swap_chain);
  }

  void TiedResources::create_or_update_global_resources()
  {
    assert(Application::render_config().valid());

    _globals.resize(Application::render_config()->globals().size());
    for (size_t idx = 0; idx < Application::render_config()->globals().size(); ++idx) {
      const RenderConfig::Resource& resource =
        Application::render_config()->globals()[idx];
      switch (resource.type) {
        case RenderConfig::Resource::RENDER_TARGET:
          if (_globals[idx])
            ((RenderTarget*)_globals[idx])->destroy();
          _globals[idx] = nullptr;
          if (Application::globals()[idx]) {
            const uint32_t width =
              _swap_chain->width() * resource.render_or_depth_stencil_target.scale.x;
            const uint32_t height =
              _swap_chain->height() * resource.render_or_depth_stencil_target.scale.y;
            Texture* alias = ((Texture*)Application::globals()[idx])->alias(
              width, height, 1, Texture::RENDER_TARGETABLE);
            _globals[idx] = (void*)RenderTarget::create(alias); }
          break;
        case RenderConfig::Resource::DEPTH_STENCIL_TARGET:
          if (_globals[idx])
            ((RenderTarget*)_globals[idx])->destroy();
          _globals[idx] = nullptr;
          if (Application::globals()[idx]) {
            const uint32_t width =
              _swap_chain->width() * resource.render_or_depth_stencil_target.scale.x;
            const uint32_t height =
              _swap_chain->height() * resource.render_or_depth_stencil_target.scale.y;
            Texture* alias = ((Texture*)Application::globals()[idx])->alias(
              width, height, 1, Texture::DEPTH_STENCIL_TARGETABLE);
            _globals[idx] = (void*)DepthStencilTarget::create(alias); }
          break;
        default:
          __builtin_unreachable();
      }
    }
  }

  void TiedResources::destroy_global_resources()
  {
    assert(Application::render_config().valid());

    for (size_t idx = 0; idx < Application::render_config()->globals().size(); ++idx) {
      const RenderConfig::Resource& resource =
        Application::render_config()->globals()[idx];
      switch (resource.type) {
        case RenderConfig::Resource::RENDER_TARGET:
          if (_globals[idx])
            ((RenderTarget*)_globals[idx])->destroy();
          _globals[idx] = nullptr;
          break;
        case RenderConfig::Resource::DEPTH_STENCIL_TARGET:
          if (_globals[idx])
            ((DepthStencilTarget*)_globals[idx])->destroy();
          _globals[idx] = nullptr;
          break;
        default:
          __builtin_unreachable();
      }
    }
  }

  void TiedResources::destroy()
  {
    make_delete(TiedResources, allocator(), this);
  }
} // butane
