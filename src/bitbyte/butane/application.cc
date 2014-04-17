//===-- bitbyte/butane/application.cc ---------------------------*- C++ -*-===//
//
//  Butane, a data-driven game engine.
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mwilliams@bitbyte.ca>
//
//===----------------------------------------------------------------------===//

#include <bitbyte/butane/application.h>

//===----------------------------------------------------------------------===//

#include <stdlib.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//

Application::Application() {}

//===----------------------------------------------------------------------===//

#if 0
Application::Application(const Application &) {}
#endif

#if 0
Application &Application::operator=(const Application &) { return *this; }
#endif

//===----------------------------------------------------------------------===//

Application::~Application() {}

//===----------------------------------------------------------------------===//

bool Application::initialize()
{
  bitbyte_butane_assert(always, "Application::initialize() is not overriden!");
  return false;
}

void Application::update(const float delta_time)
{
  bitbyte_butane_assert(always, "Application::update() is not overriden!");
}

void Application::render() const
{
  bitbyte_butane_assert(always, "Application::render() is not overriden!");
}

void Application::shutdown()
{
  bitbyte_butane_assert(always, "Application::shutdown() is not overriden!");
}

//===----------------------------------------------------------------------===//

void Application::start()
{
  const bool initialized = this->initialize();
  if (!initialized)
    bitbyte_butane_assert(always, "Application failed to initialize!");
  this->run();
}

void Application::run()
{
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

void Application::quit()
{
  this->shutdown();
  ::exit(EXIT_SUCCESS);
}

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//
