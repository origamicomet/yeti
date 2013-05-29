// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/pixel_shader.h>

namespace butane {
  const Resource::Type PixelShader::type(
    "pixel shader", "ps",
    (Resource::Type::Load)&PixelShader::load,
    (Resource::Type::Unload)&PixelShader::unload,
    (Resource::Type::Compile)&PixelShader::compile);

  PixelShader::PixelShader(
    const Resource::Id id
  ) : butane::Resource(PixelShader::type, id)
  {
  }

  PixelShader::~PixelShader()
  {
  }
} // butane
