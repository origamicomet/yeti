//===-- yeti/renderer/viewport.h ------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDERER_VIEWPORT_H_
#define _YETI_RENDERER_VIEWPORT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

// See `yeti/window.h`.
class Window;

namespace graphics {
  // See `yeti/graphics/surface.h`.
  class Surface;
}

namespace renderer {

struct Viewport {
  Window *window;
  graphics::Surface *surface;
};

} // renderer
} // yeti


#endif // _YETI_RENDERER_VIEWPORT_H_
