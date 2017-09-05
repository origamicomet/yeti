//===-- yeti/renderer.cc -------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/renderer.h"

#include "yeti/task.h"

#include "yeti/tasks/render/prepare_frame_task.h"
#include "yeti/tasks/render/submit_frame_task.h"

#if 0

enum Types {
  BACKBUFFER,
  RENDER_TARGET,
  DEPTH_STENCIL_TARGET,
  ...
};

// * We can reuse a `RenderPackage` for multi-pass rendering.

struct RenderCommand {
  // Sorting.
  u64 key;

  //
  u32 hints;

  const Package *package;
};


void set_viewport_command(u32 x, u32 y, u32 w, u32 h);
void set_scissor_command(u32 x, u32 y, u32 w, u32 h);
void set_render_and_depth_stencil_targets(...);

void clear_render_targets(...);
void clear_depth_stencil_target(...);

void set_pipeline(...);
void set_input_layout(...);
void set_topology(...);
void set_shader(...);

void draw(Buffer index_buffer,
          Buffer vertex_buffer,
          Buffer constants[8],
          Texture textures[8],
          Sampler samplers[8],
          u32 offset,
          u32 primitives);

void discard(...);

// Constants
 // How to track at high-level to improve performance?
 // Only submit dirty constants?
  // Rely on a specified rate of change.
   // Frame    = Changed once a frame.
   // Layer    = Changed once a layer.
   // Pass     = Changed once a pass.
   // Instance = Changed once an instance.

// Wrapper around `gala_command_buffer_t`?

struct RenderPackage {
  u32 length;
  u8 data[0];
};


struct RenderPackageChunk {
  u32 cursor;
  u8 raw[65535];
};

// If render_package.length > sizeof(chunk.raw) - chunk.cursor
 // Allocate a new chunk.

struct RenderPackageStream {
  RenderPackageChunk *chunks[n];
};

namespace yeti {

namespace {
  static const size_t TRANSIENT_MEMORY_PER_FRAME = 1048576;
  static const size_t FRAMES_ALLOWED_AT_ONCE = 8;

  // TODO(mtwilliams): Dynamically allocate upfront, to allow configuration.
  static u8 transient_mem_[FRAMES_ALLOWED_AT_ONCE][TRANSIENT_MEMORY_PER_FRAME];
}

Renderer::Renderer()
  : configuration_(NULL)
  , viewports_(foundation::heap(), 1)
  , id_to_layer_(foundation::heap(), 0)
  , id_to_generator_(foundation::heap(), 0)
  , id_to_modifiers_(foundation::heap(), 0)
  , layers_(foundation::heap(), 0)
  , generators_(foundation::heap(), 0)
  , modifiers_(foundation::heap(), 0)
  , global_resource_set_(NULL)
  , local_resource_sets_(foundation::heap(), 0)
  , frames_(foundation::heap(), FRAMES_ALLOWED_AT_ONCE)
  , inflight_(0) {
}

Renderer::~Renderer() {
}

void Renderer::configure(RenderConfigResource *render_config_resource) {
  yeti_assert_debug(render_config_resource != NULL);

  this->wait_until_safe();

  if (configuration_)
    configuration_->deref();

  configuration_ = configuration;

  id_to_layer_.reset();
  id_to_generator_.reset();
  id_to_modifiers_.reset();

  layers_.resize(0);
  generators_.resize(0);
  modifiers_.resize(0);

  global_resource_set_->destroy();

  for (ResourceSet **local_resource_set = local_resource_sets_.first();
       local_resource_set <= local_resource_sets_.last();
       ++local_resource_set) {
    (*local_resource_set)->destroy();
  }
}

Viewport *Renderer::create_a_viewport(Window *window) {
}

void Renderer::destroy_a_viewport(Viewport *viewport) {
}

u32 Renderer::render(const World *world,
                     Camera::Handle camera,
                     Camera::Handle viewer,
                     Viewport::Id viewport,
                     u32 *increment_after_reflection,
                     u32 *increment_after_submission,
                     u32 *increment_after_presentation) {
  // Each frame is given a globally unique identifier.
  static u32 id = 1;

  Frame *frame = frames_.acquire();

  frame->id = id++;

  frame->renderer = this;

  frame->viewport = &viewports_[viewer];

  // TODO(mtwilliams): Resource sets.
  frame->global_resource_set = NULL;
  frame->local_resource_set = NULL;

  // We don't maintain a visual representation for cameras. So we have to copy
  // the necessary data upfront.
  world->camera_system()->describe(camera, &frame->camera);
  world->camera_system()->describe(viewer, &frame->viewer);

  frame->increment_after_reflection = increment_after_reflection;
  frame->increment_after_submission = increment_after_submission;
  frame->increment_after_presentation = increment_after_presentation;

  // HACK(mtwilliams): Easiest way to initialize in place.
  uintptr_t memory = (uintptr_t)&transient_mem_[frame->id % FRAMES_ALLOWED_AT_ONCE];
  new ((void *)&frame->allocator) thread_safe::BumpAllocator(memory, TRANSIENT_MEMORY_PER_FRAME);

  // Build task graph.
  Task *prepare_frame_task =
    task::describe((Task::Kernel)&yeti::prepare_frame_task,
                   (uintptr_t)frame);

  Task *submit_frame_task =
    task::describe((Task::Kernel)&yeti::submit_frame_task,
                   (uintptr_t)frame);

  Task *house_keeping_task =
    task::describe((Task::Kernel)&Renderer::finished,
                   (uintptr_t)frame);

  task::permits(prepare_frame_task, submit_frame_task);
  task::permits(submit_frame_task, house_keeping_task);

  // Kick.
  atomic::increment(&inflight_);
  task_scheduler::submit(kick_off_rendering_task);

  return frame->id;
}

void Renderer::wait_until_safe() {
  // PERF(mtwilliams): Signal an event when no more frames are inflight.
  while (atomic::load(&inflight_) >= FRAMES_ALLOWED_AT_ONCE)
    foundation::Thread::yield();
}

void Renderer::finished(renderer::Frame *frame) {
  atomic::decrement(&frame->renderer->inflight_);

  // Return to pool.
  frame->renderer->frames_.release(frame);
}

} // yeti

#endif
