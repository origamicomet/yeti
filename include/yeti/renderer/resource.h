//===-- yeti/renderer/resource.h --------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_RENDERER_RESOURCE_H_
#define _YETI_RENDERER_RESOURCE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/graphics/texture.h"
#include "yeti/graphics/render_target.h"
#include "yeti/graphics/depth_stencil_target.h"

namespace yeti {
namespace renderer {

///
struct Resource {
  typedef u32 Id;

  enum Type {
    /// A one, two, or three dimensional texture.
    TEXTURE = 1,

    /// A render target.
    RENDER_TARGET = 2,

    /// A depth-stencil target.
    DEPTH_STENCIL_TARGET = 3,

    /// A vertex shader.
    VERTEX_SHADER = 4,

    /// A pixel shader.
    PIXEL_SHADER = 5,

    /// A compute shader.
    COMPUTE_SHADER = 6,

    /// \internal Force `u32` storage and alignment.
    YETI_RENDERER_RESOURCE_TYPE_FORCE_U32 = 0x7fffffff
  };

  Type type;

  Id id;

  const char *name;

#if 0
  union {
    Texture texture;
    RenderTarget render_target;
    DepthStencilTarget depth_stencil_target;
  };
#endif
};

} // renderer
} // yeti

#endif // _YETI_RENDERER_RESOURCE_H_
