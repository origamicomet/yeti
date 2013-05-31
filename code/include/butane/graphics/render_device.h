// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_DEVICE_H_
#define _BUTANE_RENDER_DEVICE_H_

#include <butane/butane.h>
#include <butane/graphics/render_context.h>

namespace butane {
  class BUTANE_EXPORT RenderDevice abstract {
    __foundation_trait(RenderDevice, non_copyable);

    protected:
      RenderDevice();
      virtual ~RenderDevice();

    public:
      static RenderDevice* create(
        const uint32_t adapter );

      virtual void destroy() = 0;

    public:
      virtual void dispatch(
        size_t num_render_contexts,
        const RenderContext** render_contexts ) = 0;
  };
} // butane

#endif // _BUTANE_RENDER_DEVICE_H_
