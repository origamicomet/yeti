// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASKS_GENERATE_RENDER_COMMANDS_H_
#define _BUTANE_TASKS_GENERATE_RENDER_COMMANDS_H_

#include <butane/butane.h>
#include <butane/task.h>
#include <butane/math.h>
#include <butane/graphics/viewport.h>
#include <butane/visual_representation.h>

namespace butane {
  class World;
  class SwapChain;
  class RenderContext;
namespace Tasks {
  struct GenerateRenderCommandsData {
    const World* world;
    SwapChain* swap_chain;
    const VisualRepresentation* camera;
    Viewport viewport;
    RenderContext* render_context;
    const Array<VisualRepresentation::Culled>* culled;
  };

  extern BUTANE_EXPORT void generate_render_commands(
    Task* task,
    uintptr_t data );
} // Tasks
} // butane

#endif // _BUTANE_TASKS_GENERATE_RENDER_COMMANDS_H_
