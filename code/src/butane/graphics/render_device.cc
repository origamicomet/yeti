// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/render_device.h>

#include <butane/graphics/swap_chain.h>
#include <butane/graphics/render_target.h>
#include <butane/graphics/depth_stencil_target.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("render device", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  Allocator& RenderDevice::allocator() {
    return __ts_allocator();
  }

  RenderDevice::RenderDevice()
    : _globals(allocator())
    , _swap_chains(allocator())
  {
  }

  RenderDevice::~RenderDevice()
  {
  }

  void RenderDevice::minimum_dimensions_to_support(
    uint32_t& width,
    uint32_t& height )
  {
    width = height = 0;
    for (auto iter = _swap_chains.begin(); iter != _swap_chains.end(); ++iter) {
      width = max(width, (*iter)->width());
      height = max(height, (*iter)->height());
    }
  }

  void RenderDevice::create_or_update_global_resources()
  {
    if (!_render_config.valid())
      return;

    // TODO: Make RenderTargetViews reference a smaller section of shared
    //       buffers. Expose the functionality to do so through Textures and
    //       RenderTarget::create(texture, bounds).

    uint32_t minimum_width_to_support, minimum_height_to_support; {
      minimum_dimensions_to_support(
        minimum_width_to_support,
        minimum_height_to_support); }

    _globals.resize(_render_config->globals().size());
    for (size_t idx = 0; idx < _render_config->globals().size(); ++idx) {
      const RenderConfigResource::Resource& resource =
        _render_config->globals()[idx];
      switch (resource.type) {
        case RenderConfigResource::Resource::RENDER_TARGET:
          if (_globals[idx])
            ((RenderTarget*)_globals[idx])->destroy();
          if (!_swap_chains.empty()) {
            _globals[idx] = (void*)RenderTarget::create(
              resource.render_or_depth_stencil_target.format,
              minimum_width_to_support * resource.render_or_depth_stencil_target.scale.x,
              minimum_height_to_support * resource.render_or_depth_stencil_target.scale.y); }
          break;
        case RenderConfigResource::Resource::DEPTH_STENCIL_TARGET:
          if (_globals[idx])
            ((DepthStencilTarget*)_globals[idx])->destroy();
          if (!_swap_chains.empty()) {
            _globals[idx] = (void*)DepthStencilTarget::create(
              resource.render_or_depth_stencil_target.format,
              minimum_width_to_support * resource.render_or_depth_stencil_target.scale.x,
              minimum_height_to_support * resource.render_or_depth_stencil_target.scale.y); }
          break;
        default:
          __builtin_unreachable();
      }
    }
  }

  void RenderDevice::destroy_global_resources()
  {
    if (!_render_config.valid())
      return;

    for (size_t idx = 0; idx < _render_config->globals().size(); ++idx) {
      const RenderConfigResource::Resource& resource =
        _render_config->globals()[idx];
      switch (resource.type) {
        case RenderConfigResource::Resource::RENDER_TARGET:
          if (_globals[idx])
            ((RenderTarget*)_globals[idx])->destroy();
          break;
        case RenderConfigResource::Resource::DEPTH_STENCIL_TARGET:
          if (_globals[idx])
            ((DepthStencilTarget*)_globals[idx])->destroy();
          break;
        default:
          __builtin_unreachable();
      }
    }
  }

  void RenderDevice::on_swap_chain_created(
    const SwapChain* swap_chain )
  {
    _swap_chains += swap_chain;
    create_or_update_global_resources();
  }

  void RenderDevice::on_swap_chain_resized(
    const SwapChain* swap_chain )
  {
    create_or_update_global_resources();
  }

  void RenderDevice::on_swap_chain_destroyed(
    const SwapChain* swap_chain )
  {
    for (auto iter = _swap_chains.begin(); iter != _swap_chains.end(); ++iter) {
      if ((*iter) != swap_chain)
        continue;
      _swap_chains.remove(iter);
      break;
    }

    create_or_update_global_resources();
  }

  void RenderDevice::set_render_config(
    const Resource::Handle<RenderConfigResource>& render_config )
  {
    destroy_global_resources();
    _render_config = render_config;
    create_or_update_global_resources();
  }

  Task::Affinity RenderDevice::affinity()
  {
    return ((Task::Affinity)1u << (Task::Scheduler::num_of_worker_treads() - 1));
  }
} // butane
