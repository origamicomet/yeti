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

      /*! */
      virtual void destroy() = 0;

    public:
      static Task::Affinity affinity();
  };
} // butane

#endif // _BUTANE_RENDER_DEVICE_H_
