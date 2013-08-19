// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/pixel_shader.h>

namespace butane {
  static const Resource::Type* __type_initializer() {
    return new Resource::Type(
      "pixel shader", "ps",
      (Resource::Type::Load)&PixelShader::load,
      (Resource::Type::Unload)&PixelShader::unload,
      (Resource::Type::Compile)&PixelShader::compile);
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& PixelShader::type() {
    return __ts_type();
  }

  PixelShader::PixelShader(
    const Resource::Id id
  ) : butane::Resource(PixelShader::type(), id)
  {
  }

  PixelShader::~PixelShader()
  {
  }
} // butane
