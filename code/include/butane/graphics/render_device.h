// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_DEVICE_H_
#define _BUTANE_RENDER_DEVICE_H_

#include <butane/butane.h>
#include <butane/task.h>
#include <butane/render_config.h>

namespace butane {
  class SwapChain;
  class RenderContext;
  class BUTANE_EXPORT RenderDevice abstract {
    __foundation_trait(RenderDevice, non_copyable);

    protected:
      static Allocator& allocator();

    protected:
      RenderDevice();
      virtual ~RenderDevice();

    public:
      /*! */
      static RenderDevice* create(
        const uint32_t adapter );

      /*! */
      virtual void dispatch(
        size_t num_render_contexts,
        const RenderContext** render_contexts ) = 0;

    private:
      /*! */
      void minimum_dimensions_to_support(
        uint32_t& width,
        uint32_t& height );

      /*! */
      void create_or_update_global_resources();

      /*! */
      void destroy_global_resources();

    public: /* private */
      /*! */
      void on_swap_chain_created(
        const SwapChain* swap_chain );

      /*! */
      void on_swap_chain_resized(
        const SwapChain* swap_chain );

      /*! */
      void on_swap_chain_destroyed(
        const SwapChain* swap_chain );

    public:
      /*! */
      virtual void destroy() = 0;

    public:
      static Task::Affinity affinity();

      FOUNDATION_INLINE const Resource::Handle<RenderConfig>& render_config() const
      { return _render_config; }

      void set_render_config(
        const Resource::Handle<RenderConfig>& render_config );

      FOUNDATION_INLINE const Array<void*>& globals() const
      { return _globals; }

    private:
      Resource::Handle<RenderConfig> _render_config;
      Array<void*> _globals;
      Array<const SwapChain*> _swap_chains;
  };
} // butane

#endif // _BUTANE_RENDER_DEVICE_H_
