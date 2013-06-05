// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SHADER_RESOURCE_H_
#define _BUTANE_SHADER_RESOURCE_H_

#include <butane/resource.h>

namespace butane {
  class BUTANE_EXPORT ShaderResource final
    : public butane::Resource
  {
    __foundation_trait(ShaderResource, non_copyable);

    private:
      struct MemoryResidentData {
        Hash<uint32_t, murmur_hash> layer;
        Resource::Id vertex_shader;
        Resource::Id pixel_shader;
      };

    public:
      static const Resource::Type type;

    private:
      ShaderResource(
        const Resource::Id id );

      ~ShaderResource();

    private:
      static ShaderResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        ShaderResource* config );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      FOUNDATION_INLINE Hash<uint32_t, murmur_hash> layer() const
      { return _layer; }

      FOUNDATION_INLINE Resource::Id vertex_shader() const
      { return _vertex_shader; }

      FOUNDATION_INLINE Resource::Id pixel_shader() const
      { return _pixel_shader; }

    private:
      Hash<uint32_t, murmur_hash> _layer;
      Resource::Id _vertex_shader;
      Resource::Id _pixel_shader;
  };
} // butane

#endif // _BUTANE_SHADER_RESOURCE_H_
