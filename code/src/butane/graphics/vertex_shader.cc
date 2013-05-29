// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/vertex_shader.h>

namespace butane {
  const Resource::Type VertexShader::type(
    "vertex shader", "vs",
    (Resource::Type::Load)&VertexShader::load,
    (Resource::Type::Unload)&VertexShader::unload,
    (Resource::Type::Compile)&VertexShader::compile);

  VertexShader::VertexShader(
    const Resource::Id id
  ) : butane::Resource(VertexShader::type, id)
  {
  }

  VertexShader::~VertexShader()
  {
  }
} // butane
