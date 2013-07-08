// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASKS_DISPATCH_H_
#define _BUTANE_TASKS_DISPATCH_H_

#include <butane/butane.h>
#include <butane/task.h>

namespace butane {
  class RenderContext;
namespace Tasks {
  struct DispatchData {
    size_t num_of_render_contexts;
    const RenderContext* render_contexts[BUTANE_MAXIMUM_NUM_OF_DISPATCHABLE_RENDER_CONTEXTS];
  };

  extern BUTANE_EXPORT void dispatch(
    Task* task,
    uintptr_t data );
} // Tasks
} // butane

#endif // _BUTANE_TASKS_DISPATCH_H_
