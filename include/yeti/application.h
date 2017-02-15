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

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

class Window;
class TimeStepPolicy;

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

 public:
  // TODO(mtwilliams): Document this interface.
  virtual bool startup();
  virtual void shutdown();
  virtual void update(const f32 delta_time);
  virtual void render();

  void run();

  void pause();
  void unpause();

  void quit();

 private:
  static void window_event_handler_(void *);

 public:
  // TODO(mtwilliams): Document this interface.
  foundation::Array<Window *> &windows();
  const foundation::Array<Window *> &windows() const;

 public:
  // TODO(mtwilliams): Document this interface.
  TimeStepPolicy *time_step_policy();
  const TimeStepPolicy *time_step_policy() const;
  void set_time_step_policy(TimeStepPolicy *new_time_step_policy);

 protected:
  foundation::Array<Window *> windows_;
  TimeStepPolicy *time_step_policy_;
};

} // yeti

#endif // _YETI_APPLICATION_H_
