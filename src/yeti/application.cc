//===-- yeti/application.cc -------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/application.h"

#include "yeti/window.h"
#include "yeti/time_step_policy.h"
#include "yeti/input.h"

namespace yeti {

Application::Application()
  : windows_(foundation::heap())
  /* , time_step_policy_(NULL) */ {
}

Application::~Application() {
  // There's no need to clean up after ourselves as the operating system will
  // do that for us. Ever use software that takes more than a second to close?
  // That's because they follow "best" practices and make sure everything is
  // properly destructed and free'd before they exit.
}

const char *Application::platform() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return "windows";
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  return "macosx";
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return "linux";
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
  return "ios";
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return "android";
#endif
}

const char *Application::architecture() {
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  return "x86";
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  return "x86_64";
#endif
}

const char *Application::build() {
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
  return "debug";
#elif YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  return "development";
#elif YETI_CONFIGURATION == YETI_CONFIGURATION_RELEASE
  return "release";
#endif
}

bool Application::startup() {
  return true;
}

void Application::shutdown() {
}

void Application::update(const f32 delta_time) {
  yeti_assert_debug(delta_time >= 0.f);
}

void Application::render() {
}

void Application::run() {
  // TODO(mtwilliams): Move this into a task?
  foundation::HighResolutionTimer *frame = foundation::HighResolutionTimer::create();
  foundation::HighResolutionTimer *wall = foundation::HighResolutionTimer::create();

  this->startup();

  for (;;) {
    yeti_assert_debug(time_step_policy_ != NULL);
    time_step_policy_->update(frame, wall);

    for (Window **window = windows_.first(); window != windows_.last(); ++window)
      (*window)->update(&window_event_handler_, (void *)this);

    // It's important that these are pulled out prior to updating, just in case
    // the time-step policy is changed during a step.
    const u32 steps = time_step_policy_->steps();
    const f32 delta_time_per_step = time_step_policy_->delta_time_per_step();

    for (u32 step = 0; step < steps; ++step)
      this->update(delta_time_per_step);

    this->render();

    yeti::Keyboard::update();
    yeti::Mouse::update();

    // BUG(mtwilliams): Due to inadequate timing resolution, this may result in
    // all frames taking `zero' microseconds unless enough work is performed
    // every frame.
    frame->reset();
  }

  YETI_TRAP();
}

void Application::pause() {
  yeti_assertf(0, "Pausing and unpausing applications has not been implemented yet.");
}

void Application::unpause() {
  yeti_assertf(0, "Pausing and unpausing applications has not been implemented yet.");
}

void Application::quit() {
  this->shutdown();
  ::exit(EXIT_SUCCESS);
  YETI_UNREACHABLE();
}

void Application::window_event_handler_(void *ctx) {
  Window *window = (Window *)ctx;
  YETI_UNUSED(window);
}

Window **Application::windows() {
  return (Window **)windows_.first();
}

const Window **Application::windows() const {
  return (const Window **)windows_.first();
}

u32 Application::num_windows() const {
  return windows_.size();
}

TimeStepPolicy *Application::time_step_policy() {
  return time_step_policy_;
}

const TimeStepPolicy *Application::time_step_policy() const {
  return time_step_policy_;
}

void Application::set_time_step_policy(TimeStepPolicy *new_time_step_policy) {
  if (time_step_policy_)
    time_step_policy_->destroy();
  time_step_policy_ = new_time_step_policy;
}

} // yeti
