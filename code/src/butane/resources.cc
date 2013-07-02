// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources.h>
#include <butane/resources/config.h>
#include <butane/resources/mesh.h>
#include <butane/graphics/pixel_shader.h>
#include <butane/resources/shader.h>
#include <butane/resources/state.h>
#include <butane/resources/texture.h>
#include <butane/resources/unit.h>
#include <butane/graphics/vertex_shader.h>

namespace butane {
namespace Resources {
  void expose() {
    ConfigResource::type().expose();
    MeshResource::type().expose();
    PixelShader::type().expose();
    ShaderResource::type().expose();
    StateResource::type().expose();
    TextureResource::type().expose();
    VertexShader::type().expose();
  }
} // Resources
} // butane
