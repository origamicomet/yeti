// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VERTEX_SHADER_H_
#define _BUTANE_VERTEX_SHADER_H_

#include <butane/resource.h>
#include <butane/graphics/vertex_declaration.h>

namespace butane {
  class BUTANE_EXPORT VertexShader
    : public butane::Resource
  {
    __foundation_trait(VertexShader, non_copyable);

    private:
      struct MemoryResidentData {
        VertexDeclaration vertex_declaration;
        uint32_t byte_code_len;
        uint8_t byte_code[1];
      };

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
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      FOUNDATION_INLINE const VertexDeclaration& vertex_declaration() const
      { return _vertex_declaration; }

    protected:
      VertexDeclaration _vertex_declaration;
  };
} // butane

#endif // _BUTANE_VERTEX_SHADER_H_
