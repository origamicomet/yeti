// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_STATE_RESOURCE_H_
#define _BUTANE_STATE_RESOURCE_H_

#include <butane/resource.h>
#include <butane/graphics/rasterizer_state.h>
#include <butane/graphics/depth_stencil_state.h>
#include <butane/graphics/blend_state.h>

namespace butane {
  class RasterizerState;
  class DepthStencilState;
  class BlendState;

  class BUTANE_EXPORT StateResource final
    : public butane::Resource
  {
    __foundation_trait(StateResource, non_copyable);

    private:
      struct MemoryResidentData {
        RasterizerState::Desc rsd;
        DepthStencilState::Desc dssd;
        BlendState::Desc bsd;
      };

    public:
      static const Resource::Type type;

    private:
      StateResource(
        const Resource::Id id );

      ~StateResource();

    private:
      static StateResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        StateResource* state );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      FOUNDATION_INLINE RasterizerState* rasterizer_state() const
      { return _rasterizer_state; }

      FOUNDATION_INLINE DepthStencilState* depth_stencil_state() const
      { return _depth_stencil_state; }

      FOUNDATION_INLINE BlendState* blend_state() const
      { return _blend_state; }

    private:
      RasterizerState* _rasterizer_state;
      DepthStencilState* _depth_stencil_state;
      BlendState* _blend_state;
  };
} // butane

#endif // _BUTANE_STATE_RESOURCE_H_
