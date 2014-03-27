//=== bitbyte/butane/butane.cc ===============================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

#include <bitbyte/butane/application.h>

#include <stdlib.h>

namespace bitbyte {
namespace butane {

Application::Application() {
}

Application::Application(const Application &application) {
}

Application &Application::operator=(const Application &application) {
  return *this;
}

Application::~Application() {
}

const char *Application::platform() const {
#if (BITBYTE_FOUNDATION_TARGET_PLATFORM == BITBYTE_FOUNDATION_PLATFORM_WINDOWS)
  return "windows";
#elif (BITBYTE_FOUNDATION_TARGET_PLATFORM == BITBYTE_FOUNDATION_PLATFORM_MACOSX)
  return "macosx";
#elif (BITBYTE_FOUNDATION_TARGET_PLATFORM == BITBYTE_FOUNDATION_PLATFORM_LINUX)
  return "linux";
#elif (BITBYTE_FOUNDATION_TARGET_PLATFORM == BITBYTE_FOUNDATION_PLATFORM_IOS)
  return "ios";
#elif (BITBYTE_FOUNDATION_TARGET_PLATFORM == BITBYTE_FOUNDATION_PLATFORM_ANDROID)
  return "android";
#endif
}

const char *Application::build() const {
#if (BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_DEBUG)
  return "debug";
#elif (BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_DEVELOPMENT)
  return "development";
#elif (BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_RELEASE)
  return "release";
#endif
}

bool Application::initialize() {
  bitbyte_butane_assert(always, "Application::initialize() is not overriden!");
  return false;
}

void Application::update(const float delta_time) {
  bitbyte_butane_assert(always, "Application::update() is not overriden!");
}

void Application::render() const {
  bitbyte_butane_assert(always, "Application::render() is not overriden!");
}

void Application::shutdown() {
  bitbyte_butane_assert(always, "Application::shutdown() is not overriden!");
}

void Application::run() {
  this->initialize();
  // foundation::MonotonicClock wall, frame;
  // while (true) {
  //   TimeStepPolicy &time_step_policy = this->time_step_policy();
  //   time_step_policy.update(wall, frame);
  //   for (size_t step = 0; step < time_step_policy.steps(); ++step)
  //     this->update(time_step_policy.delta_time_per_step());
  //   this->render();
  //   frame.reset();
  // }
}

void Application::quit() {
  this->shutdown();
  ::exit(EXIT_SUCCESS);
}

} // butane
} // bitbyte
