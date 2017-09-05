//===-- yeti/graphics/surface.cc ------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/graphics/surface.h"

#include "yeti/window.h"

#include "yeti/graphics/engine.h"
#include "yeti/graphics/transient_command_buffer.h"

// TODO(mtwilliams): Drop auto-submitting `TransientCommandBuffer` wrappers?

namespace yeti {
namespace graphics {

Surface::Surface()
  : swap_chain_(GALA_INVALID_SWAP_CHAIN_HANDLE)
  , render_target_view_(GALA_INVALID_RENDER_TARGET_VIEW_HANDLE)
{
}

Surface::~Surface() {
}

Surface *Surface::create_from_window(yeti::Window *window) {
  TransientCommandBuffer commands;
  Surface *surface = Surface::create_from_window(window, &commands);
  commands.submit(true);
  return surface;
}

Surface *Surface::create_from_window(yeti::Window *window,
                                     ::gala_command_buffer_t *commands) {
  yeti_assert_debug(window != NULL);
  yeti_assert_debug(commands != NULL);

  u32 width, height;
  window->dimensions(&width, &height);

  Surface *surface = new (foundation::heap()) Surface();

  gala_swap_chain_desc_t swap_chain_desc;
  swap_chain_desc.format = GALA_PIXEL_FORMAT_R8G8B8;
  swap_chain_desc.width = width;
  swap_chain_desc.height = height;
  swap_chain_desc.refresh_rate.numerator = 60;
  swap_chain_desc.refresh_rate.denominator = 1;
  swap_chain_desc.flags = GALA_SWAP_CHAIN_SYNCHRONIZE;
  swap_chain_desc.surface = window->to_native_hndl();

  surface->swap_chain_ = gala_swap_chain_create(engine::underlying(), commands, &swap_chain_desc);

  gala_render_target_view_desc_t render_target_view_desc;
  render_target_view_desc.format = swap_chain_desc.format;
  render_target_view_desc.dimensionality = GALA_TWO_DIMENSIONAL;
  render_target_view_desc.width = swap_chain_desc.width;
  render_target_view_desc.height = swap_chain_desc.height;
  render_target_view_desc.storage = surface->swap_chain_;

  surface->render_target_view_ = gala_render_target_view_create(engine::underlying(), commands, &render_target_view_desc);

  return surface;
}

void Surface::destroy() {
  TransientCommandBuffer commands;
  this->destroy(&commands);
  commands.submit(true);
}

void Surface::destroy(::gala_command_buffer_t *commands) {
  yeti_assert_debug(commands != NULL);

  yeti_assert_debug(swap_chain_ != GALA_INVALID_SWAP_CHAIN_HANDLE);
  yeti_assert_debug(render_target_view_ != GALA_INVALID_RENDER_TARGET_VIEW_HANDLE);

  gala_swap_chain_destroy(engine::underlying(), commands, swap_chain_);
  gala_render_target_view_destroy(engine::underlying(), commands, render_target_view_);

  delete this;
}

} // graphics
} // yeti
