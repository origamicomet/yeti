//===-- yeti/runtime/standard_application.cc --------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/runtime/standard_application.h"

#include "yeti/runtime/manifest.h"

#include <stdlib.h>
#include <stdio.h>

namespace yeti {
namespace runtime {

StandardApplication::StandardApplication() {
}

StandardApplication::~StandardApplication() {
}

bool StandardApplication::startup() {
  // TODO(mtwilliams): Load manifest.
  this->manifest_ = NULL; /* Manifest::load("app.manifest"); */

  // Start logging to the console and to a file:
  this->start_logging_to_stdout_and_stderr();
  this->start_logging_to_file();

  // Set the appropriate logging level based on build configuration.
  this->set_appropriate_logging_level();

  // Can't say we didn't tell ya'.
  this->log_copyright_notices();

  // Engine, game, and content revisions are logged to aid when debugging and
  // filing crash reports.
  this->log_pertinent_information_about_build();

  // Hardware information is logged too.
  this->log_pertinent_information_about_system();

  // Create the main window.
  yeti::Window *window = yeti::Window::open({"Yeti", {1280, 720}});
  yeti::input::from(window);
  window->show();
  this->windows_.push(window);

  // Default to a variable time-step policy.
  this->time_step_policy_ = yeti::TimeStepPolicy::create({yeti::TimeStepPolicy::VARIABLE});

  return true;
}

void StandardApplication::update(const f32 delta_time) {
}

void StandardApplication::render() const {
}

void StandardApplication::shutdown() {
}

void StandardApplication::start_logging_to_stdout_and_stderr() const {
  // TODO(mtwilliams): Log to stdout and stderr.
}

void StandardApplication::start_logging_to_file() const {
  // TODO(mtwilliams): Log to `YYYY-MM-DD-HH-MM-SS.log`
}

void StandardApplication::set_appropriate_logging_level() const {
  // TODO(mtwilliams): Refactor start_logging_to_stdout_and_stderr() and
  // start_logging_to_file() into Console/Log and use set_logging_level().
}

void StandardApplication::log_copyright_notices() const {
  ::printf(
    "                                                                              \n"
    "                               __ __     _   _                                \n"
    "                              |  |  |___| |_|_|                               \n"
    "                              |_   _| -_|  _| |                               \n"
    "                                |_| |___|_| |_|                               \n"
    "                                                                              \n"
                       "      %s      \n\n", __yeti_copyright__()
  );
}

void StandardApplication::log_pertinent_information_about_build() const {
  ::printf("  Runtime\n");
  ::printf("    platform=%s\n", platform());
  ::printf("    architecture=%s\n", architecture());
  ::printf("    build=%s\n\n", build());

  // TODO(mtwilliams): Determine content version.
  ::printf("  Runtime\n");
  ::printf("    engine=%s\n", __yeti_version__());
  ::printf("    runtime=%s\n", __yeti_version__());
  ::printf("    content=%s\n\n", "unknown");
}

void StandardApplication::log_pertinent_information_about_system() const {
  // TODO(mtwilliams): Log CPU and GPU information.
}

} // runtime
} // yeti
