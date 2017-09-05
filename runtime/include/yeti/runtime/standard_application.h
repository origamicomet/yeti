//===-- yeti/runtime/standard_application.h -------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RUNTIME_STANDARD_APPLICATION_H_
#define _YETI_RUNTIME_STANDARD_APPLICATION_H_

#include "yeti.h"

namespace yeti {
namespace runtime {

// See `yeti/runtime/manifest.h`.
struct Manifest;

class StandardApplication : public yeti::Application {
 YETI_DISALLOW_COPYING(StandardApplication)

 public:
  StandardApplication(const Manifest *manifest);
  ~StandardApplication();

 protected:
  bool startup();
  void update(const f32 delta_time);
  void render();
  void shutdown();

 private:
  // Starts logging to console.
  void start_logging_to_console() const;

  // Starts logging to a file.
  void start_logging_to_file() const;

  // Sets the accepted logging levels based on build configuration.
  void set_appropriate_logging_level() const;

  // Logs game and engine copyright information.
  void log_copyright_notices() const;

  // Logs engine, game, and content revisions.
  void log_pertinent_information_about_build() const;

  // Logs software and hardware information.
  void log_pertinent_information_about_system() const;

 private:
  // Creates main window and default viewport.
  void create_main_window_and_default_viewport();

  // Exposes interfaces to Lua.
  void expose_to_lua();

  // Loads boot package and script.
  void load_boot_package_and_script();

 private:
  const Manifest *manifest_;
  Script script_;
};

} // runtime
} // yeti

#endif // _YETI_RUNTIME_STANDARD_APPLICATION_H_
