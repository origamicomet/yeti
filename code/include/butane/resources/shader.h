// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SHADER_RESOURCE_H_
#define _BUTANE_SHADER_RESOURCE_H_

#include <butane/resource.h>
#include <butane/resources/state.h>
#include <butane/resources/render_config.h>
#include <butane/graphics/vertex_shader.h>
#include <butane/graphics/pixel_shader.h>

namespace butane {
  class BUTANE_EXPORT ShaderResource final
    : public butane::Resource
  {
    __foundation_trait(ShaderResource, non_copyable);

    private:
      struct MemoryResidentData {
        RenderConfigResource::Layer::Name layer;
        Resource::Id state;
        Resource::Id vertex_shader;
        Resource::Id pixel_shader;
      };

    public:
      static const Resource::Type& type();

    private:
      ShaderResource(
        const Resource::Id id );

      ~ShaderResource();

    public: /* private */
      static ShaderResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        ShaderResource* shader );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      FOUNDATION_INLINE Hash<uint32_t, murmur_hash> layer() const
      { return _layer; }

      FOUNDATION_INLINE Resource::Handle<StateResource> state() const
      { return _state; }

      FOUNDATION_INLINE Resource::Handle<VertexShader> vertex_shader() const
      { return _vertex_shader; }

      FOUNDATION_INLINE Resource::Handle<PixelShader> pixel_shader() const
      { return _pixel_shader; }

    private:
      RenderConfigResource::Layer::Name _layer;
      Resource::Handle<StateResource> _state;
      Resource::Handle<VertexShader> _vertex_shader;
      Resource::Handle<PixelShader> _pixel_shader;
  };
} // butane

#endif // _BUTANE_SHADER_RESOURCE_H_
