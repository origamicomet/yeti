// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/vertex_shader.h>

namespace butane {
  static const Resource::Type* __type_initializer() {
    return new Resource::Type(
      "vertex shader", "vs",
      (Resource::Type::Load)&VertexShader::load,
      (Resource::Type::Unload)&VertexShader::unload,
      (Resource::Type::Compile)&VertexShader::compile);
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& VertexShader::type() {
    return __ts_type();
  }

  VertexShader::VertexShader(
    const Resource::Id id
  ) : butane::Resource(VertexShader::type(), id)
  {
  }

  VertexShader::~VertexShader()
  {
  }
} // butane
