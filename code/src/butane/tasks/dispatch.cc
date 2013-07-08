// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/dispatch.h>

#include <butane/application.h>
#include <butane/graphics/render_device.h>
#include <butane/graphics/render_context.h>

namespace butane {
namespace Tasks {
  void dispatch(
    Task* task,
    uintptr_t data )
  {
    DispatchData* dd = (DispatchData*)data;
    RenderDevice* rd = Application::render_device();

    rd->dispatch(dd->num_of_render_contexts, &dd->render_contexts[0]);

    for (size_t i = 0; i < dd->num_of_render_contexts; ++i)
      make_delete(RenderContext, Allocators::scratch(), dd->render_contexts[i]);
    Allocators::scratch().free((void*)data);
  }
} // Tasks
} // butane
