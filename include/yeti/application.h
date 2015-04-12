//===-- yeti/application.h --------------------------------*- mode: C++ -*-===//
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

namespace yeti {

// See include/yeti/time_step_policy.h
class TimeStepPolicy;
// See include/yeti/window.h
class Window;

class YETI_PUBLIC Application {
 YETI_DISALLOW_COPYING(Application);

 protected:
  Application();
  virtual ~Application();

 public:
  void start();
  void quit();

 public:
  void pause();
  void resume();
  bool paused() const;

 public:
  virtual bool initialize() YETI_ABSTRACT;
  virtual void update(const Real32 delta_time) YETI_ABSTRACT;
  virtual void render() const YETI_ABSTRACT;
  virtual void shutdown() YETI_ABSTRACT;

 public:
  TimeStepPolicy *time_step_policy() const;
  void set_time_step_policy(TimeStepPolicy *new_time_step_policy);
  Window *window() const;
  void set_window(Window *new_window);

 private:
  TimeStepPolicy *time_step_policy_;
  Window *window_;
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  foundation::Atomic<bool> is_paused_;
#endif
};

YETI_INLINE TimeStepPolicy *Application::time_step_policy() const {
  return time_step_policy_;
}

YETI_INLINE void Application::set_time_step_policy(TimeStepPolicy *new_time_step_policy) const {
  yeti_assert_debug(new_time_step_policy != NULL);
  time_step_policy_ = new_time_step_policy;
}

YETI_INLINE Window *Application::window() const {
  return window_;
}

YETI_INLINE void Application::set_window(Window *new_window) const {
  yeti_assert_debug(new_window != NULL);
  window_ = new_window;
}

} // yeti

#endif /* _YETI_APPLICATION_H_ */

//===----------------------------------------------------------------------===//
