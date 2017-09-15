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

} // yeti

#endif // _YETI_RENDERER_H_
