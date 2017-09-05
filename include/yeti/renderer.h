//===-- yeti/renderer.h ---------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDERER_H_
#define _YETI_RENDERER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/renderer/viewport.h"

#include "yeti/renderer/resource.h"
#include "yeti/renderer/resource_set.h"
#include "yeti/renderer/resource_generator.h"
#include "yeti/renderer/resource_modifier.h"
#include "yeti/renderer/layer.h"

#include "yeti/renderer/frame.h"

namespace yeti {

// See `yeti/resources/render_config_resource.h`.
class RenderConfigResource;

class YETI_PRIVATE Renderer {
 YETI_DISALLOW_COPYING(Renderer)

 public:
  /// \copydoc yeti::renderer::Viewport
  typedef renderer::Viewport Viewport;

  /// \copydoc yeti::renderer::Resource
  typedef renderer::Resource Resource;

  /// \copydoc yeti::renderer::ResourceSet
  typedef renderer::ResourceSet ResourceSet;

  /// \copydoc yeti::renderer::ResourceGenerator
  typedef renderer::ResourceGenerator ResourceGenerator;

  /// \copydoc yeti::renderer::ResourceModifier
  typedef renderer::ResourceModifier ResourceModifier;

  /// \copydoc yeti::renderer::Layer
  typedef renderer::Layer Layer;

  /// \copydoc yeti::renderer::Frame
  typedef renderer::Frame Frame;
};

#if 0

/// ...
class Renderer {
 YETI_DISALLOW_COPYING(Renderer)

 public:
  /// \copydoc yeti::renderer::Viewport
  typedef renderer::Viewport Viewport;

  /// \copydoc yeti::renderer::Frame
  typedef renderer::Frame Frame;

 public:
  Renderer();
  ~Renderer();

 public:
  void configure(RenderConfigResource *configuration);

 public:
  Viewport *create_a_viewport(Window *window);
  void destroy_a_viewport(Viewport *viewport);

 public:
  /// \brief Kicks off rendering of a world.
  ///
  /// \param @world World to render.
  /// \param @camera Camera to build frame for.
  /// \param @viewer Camera to render frame with.
  /// \param @viewport Viewport to render to.
  ///
  /// \param @decrement_after_reflection Decremented once it is safe to modify @world.
  /// \param @increment_after_submission Incremented after the frame is submitted.
  /// \param @increment_after_presentation Incremented after the frame is presented.
  ///
  /// \returns Globally unique identifier assigned to the frame.
  ///
  u32 render(const World *world,
             Camera::Handle camera,
             Camera::Handle viewer,
             Viewport::Id viewport,
             u32 *decrement_after_reflection,
             u32 *increment_after_submission,
             u32 *increment_after_presentation);

 private:
  /// \internal Waits until inflight frames have been submitted.
  void wait_until_safe();

 private:
  /// \internal Performs house-keeping after a frame has been submitted.
  static void finished(renderer::Frame *frame);

 private:
  RenderConfigResource *configuration_;

  foundation::SparseArray<Viewport> viewports_;

  foundation::HashMap<u32, u32> id_to_layer_;
  foundation::HashMap<u32, u32> id_to_generator_;
  foundation::HashMap<u32, u32> id_to_modifier_;

  // TODO(mtwilliams): Variables.
  // foundation::Array<Variable> variables_;

  foundation::Array<Layer> layers_;
  foundation::Array<ResourceGenerator> generators_;
  foundation::Array<ResourceModifier> modifiers_;

  ResourceSet *global_resource_set_;
  foundation::Array<ResourceSet *> local_resource_sets_;

  foundation::thread_safe::Pool<Frame> frames_;

  // Number of frames inflight.
  u64 inflight_;
};

#endif

} // yeti

#endif // _YETI_RENDERER_H_

