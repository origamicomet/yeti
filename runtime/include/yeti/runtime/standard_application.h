//===-- yeti/runtime/standard_application.h ---------------*- mode: C++ -*-===//
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

class Manifest;

class StandardApplication : public yeti::Application {
 YETI_DISALLOW_COPYING(StandardApplication);

 public:
  StandardApplication();
  ~StandardApplication();

 public:
  bool startup();
  void update(const f32 delta_time);
  void render() const;
  void shutdown();

 private:
  /// \brief Starts logging to stdout/stderr.
  void start_logging_to_stdout_and_stderr() const;

  /// \brief Starts logging to a file.
  void start_logging_to_file() const;

  /// \brief Sets the accepted logging levels based on build configuration.
  void set_appropriate_logging_level() const;

  /// \brief Logs game and engine copyright information.
  void log_copyright_notices() const;

  /// \brief Logs engine, game, and content revisions.
  void log_pertinent_information_about_build() const;

  /// \brief Logs software and hardware information.
  void log_pertinent_information_about_system() const;

private:
  Manifest *manifest_;
  Script script_;
};

} // runtime
} // yeti

#endif // _YETI_RUNTIME_STANDARD_APPLICATION_H_
