//===-- yeti/renderer/frame.h ---------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_RENDERER_FRAME_H_
#define _YETI_RENDERER_FRAME_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

// We don't maintain a visual representation for cameras, so we have to copy
// the necessary data upfront. Hence embedding into `Frame`.
#include "yeti/camera.h"

namespace yeti {

// See `yeti/world.h`.
class World;

// See `yeti/renderer.h`.
class Renderer;

namespace renderer {

struct Viewport;

struct Resource;
class ResourceSet;

struct Frame {
  u64 id;

  const Renderer *renderer;

  renderer::ResourceSet *global_resource_set;
  renderer::ResourceSet *local_resource_set;

  const renderer::Viewport *viewport;

  const World *world;

  Camera camera;
  Camera viewer;

  /// Simple bump allocator used for transient allocations.
  ///
  /// \warning Annihilated after submission. Don't use for long lived
  /// allocations or expect destructors to be called.
  ///
  foundation::thread_safe::BumpAllocator allocator;

  /// Decremented after reflection, i.e. when its safe to mutate @world.
  volatile u32 *decrement_after_reflection;

  /// Incremented after submission.
  volatile u32 *increment_after_submission;

  /// Incremented after presentation.
  volatile u32 *increment_after_presentation;
};

}

} // yeti

#endif // _YETI_RENDERER_FRAME_H_
