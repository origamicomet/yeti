//===-- yeti/graphics/engine.h --------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_GFX_ENGINE_H_
#define _YETI_GFX_ENGINE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "gala.h"

namespace yeti {
namespace graphics {

namespace engine {

///
enum Backend {
  ///
  NONE = 0,

  /// Direct3D 11
  D3D11 = 1,

  /// Direct3D 12
  D3D12 = 2,

  /// OpenGL 3.3+
  OPENGL = 3,

  /// \internal Force `u32` storage and alignment.
  YETI_GFX_BACKEND_FORCE_U32 = 0x7fffffff
};

///
struct Settings {
  Backend backend;

  /// Allow abuse of undefined behaviour to increase performance.
  bool fast_and_loose;
};

///
extern YETI_PUBLIC bool supported(const Backend backend);

///
extern YETI_PUBLIC void initialize(const Settings &settings);

///
extern YETI_PUBLIC void shutdown();

///
extern YETI_PRIVATE void submit(const ::gala_command_buffer_t *command_buffer,
                              void (*executed)(void *) = NULL,
                              void *executed_ctx = NULL);

///
extern YETI_PUBLIC gala_engine_t *underlying();

}

} // graphics
} // yeti

#endif // _YETI_GFX_ENGINE_H_
