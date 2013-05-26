// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VERTEX_SHADER_RESOURCE_H_
#define _BUTANE_VERTEX_SHADER_RESOURCE_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/resource.h>
#include <butane/vertex_declaration.h>

namespace butane {
  class BUTANE_EXPORT VertexShader
    : public butane::Resource
  {
    __foundation_trait(VertexShader, non_copyable);

    public:
      static const Resource::Type type;

    protected:
      VertexShader(
        const Resource::Id id );

      virtual ~VertexShader();

    private:
      static VertexShader* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        VertexShader* vertex_shader );

      static bool compile(
        const Resource::Compiler::Source& src,
        const Resource::Compiler::Stream& cs );

    protected:
      VertexDeclaration _vd;
  };
} // butane

#endif // _BUTANE_VERTEX_SHADER_RESOURCE_H_
