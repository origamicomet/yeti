//===-- yeti/graphics/transient_command_buffer.h --------*- mode: C++11 -*-===//
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

#ifndef _YETI_GRAPHICS_TRANSIENT_COMMAND_BUFFER_H_
#define _YETI_GRAPHICS_TRANSIENT_COMMAND_BUFFER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "gala/command_buffer.h"

namespace yeti {
namespace graphics {

class YETI_PUBLIC TransientCommandBuffer : public ::gala_command_buffer_t {
 YETI_DISALLOW_COPYING(TransientCommandBuffer)

 public:
  TransientCommandBuffer();
  ~TransientCommandBuffer();

 public:
  void submit(bool force = false);

 public:
  /// \internal
  bool exhausted_();

 private:
  void *block_;
};

} // graphics
} // yeti

#endif // _YETI_GRAPHICS_TRANSIENT_COMMAND_BUFFER_H_
