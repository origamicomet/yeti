// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_PIXEL_SHADER_RESOURCE_H_
#define _BUTANE_PIXEL_SHADER_RESOURCE_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/resource.h>

namespace butane {
  class BUTANE_EXPORT PixelShader
    : public butane::Resource
  {
    __foundation_trait(PixelShader, non_copyable);

    public:
      static const Resource::Type type;

    protected:
      PixelShader(
        const Resource::Id id );

      virtual ~PixelShader();

    private:
      static PixelShader* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        PixelShader* pixel_shader );

      static bool compile(
        const Resource::Compiler::Source& src,
        const Resource::Compiler::Stream& cs );
  };
} // butane

#endif // _BUTANE_PIXEL_SHADER_RESOURCE_H_
