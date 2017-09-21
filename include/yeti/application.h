//===-- yeti/application.h ------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_APPLICATION_H_
#define _YETI_APPLICATION_H_

#include "yeti/core.h"

// Have to include because we use `Window::Description`.
#include "yeti/window.h"

// Have to include because we use `Camera::Handle`.
#include "yeti/components/camera.h"

namespace yeti {

// See `yeti/application/time_step_policy.h`.
class TimeStepPolicy;

// See `yeti/world.h`.
class World;

/// ...
class YETI_PUBLIC Application {
 YETI_DISALLOW_COPYING(Application)

 protected:
  Application();
  virtual ~Application();

 public:
  // TODO(mtwilliams): Document these.
  static const char *platform();
  static const char *architecture();
  static const char *build();

 protected:
  // TODO(mtwilliams): Document this interface.
  virtual bool startup();
  virtual void shutdown();
  virtual void update(const f32 delta_time);
  virtual void render();

 public:
  Window *open_a_window(const Window::Description &desc);

  void close_a_window(Window *window);

 private:
  static void window_event_handler_(Window *window,
                                    const Window::Event &event,
                                    void *self);

 public:
  World *create_a_world();

  void update_a_world(World *world,
                      const f32 delta_time);

#if 0
  void render_a_world(const World *world,
                      Camera::Handle camera,
                      Renderer::Viewport *viewport);
#endif

  void destroy_a_world(World *world);

 public:
  void run();

  void pause();
  void unpause();

  void quit();

 public:
  // TODO(mtwilliams): Document this interface.
  TimeStepPolicy *time_step_policy();
  const TimeStepPolicy *time_step_policy() const;
  void set_time_step_policy(TimeStepPolicy *new_time_step_policy);

 public:
  // TODO(mtwilliams): Document these interfaces.
  const core::Array<Window *> &windows() const;
  const core::Array<World *> &worlds() const;

 protected:
  TimeStepPolicy *time_step_policy_;

  core::Array<Window *> windows_;

  core::Array<World *> worlds_;

  u32 logical_frame_count_;
  u32 visual_frame_count_;
};

} // yeti

#endif // _YETI_APPLICATION_H_
