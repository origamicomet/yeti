//===-- yeti/foundation/mirrored_ring_buffer.h ----------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_MIRRORED_RING_BUFFER_H_
#define _YETI_FOUNDATION_MIRRORED_RING_BUFFER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// A mirrored ring buffer!
///
class MirroredRingBuffer {
 YETI_DISALLOW_CONSTRUCTION(MirroredRingBuffer)
 YETI_DISALLOW_COPYING(MirroredRingBuffer)

 public:
  /// \brief Creates a mirrored ring-buffer of |sz| bytes.
  static MirroredRingBuffer *create(u32 sz);

  /// \brief Destroys this mirrored ring-buffer, freeing any mapped memory.
  void destroy();

 private:
  struct Implementation;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_MIRRORED_RING_BUFFER_H_
