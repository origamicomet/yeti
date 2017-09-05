//===-- yeti/graphics/surface.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_GRAPHICS_SURFACE_H_
#define _YETI_GRAPHICS_SURFACE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "gala/resources/swap_chain.h"
#include "gala/resources/render_target_view.h"

namespace yeti {

// Defined in `yeti/window.h`
class Window;

namespace graphics {

/// ...
class YETI_PUBLIC Surface {
 YETI_DISALLOW_COPYING(Surface);

 private:
  Surface();
  ~Surface();

 public:
  // TODO(mtwilliams): Document this interface.
  static Surface *create_from_window(yeti::Window *window);
  static Surface *create_from_window(yeti::Window *window,
                                     ::gala_command_buffer_t *commands);

  void destroy();
  void destroy(::gala_command_buffer_t *commands);

 public:
  ::gala_swap_chain_handle_t swap_chain() const;
  ::gala_render_target_view_handle_t render_target_view() const;

 private:
  ::gala_swap_chain_handle_t swap_chain_;
  ::gala_render_target_view_handle_t render_target_view_;
};

YETI_INLINE ::gala_swap_chain_handle_t Surface::swap_chain() const {
  return swap_chain_;
}

YETI_INLINE ::gala_render_target_view_handle_t Surface::render_target_view() const {
  return render_target_view_;
}

} // graphics
} // yeti

#endif // _YETI_GRAPHICS_SURFACE_H_
