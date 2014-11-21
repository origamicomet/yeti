//===-- bitbyte/butane/application.c ----------------------------*- C++ -*-===//
//
//  Butane
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@bitbyte.ca>
//
//===----------------------------------------------------------------------===//

#include "bitbyte/butane/application.h"

//===----------------------------------------------------------------------===//

#include <stdlib.h>

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

void
bitbyte_butane_application_start(
  bitbyte_butane_application_t *application)
{
  bitbyte_butane_assert_debug(application != NULL);
  const bool initialized = application->initialize(application);
  bitbyte_butane_assertf(initialized, "Application initialization failed!");
  if (application->time_step_policy == NULL) {
    bitbyte_foundation_log_unformatted(
      BITBYTE_FOUNDATION_LOG_LEVEL_WARNING,
      "No time-step policy specified, defaulting to `variable`!");
    bitbyte_butane_time_step_policy_opts_t tspo;
    tspo.type = BITBYTE_BUTANE_TIME_STEP_POLICY_VARIABLE;
    application->time_step_policy = bitbyte_butane_time_step_policy_create(&tspo);
  }
  bitbyte_butane_application_run(application);
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_application_run(
  bitbyte_butane_application_t *application)
{
  bitbyte_butane_assert_debug(application != NULL);

  bitbyte_foundation_timer_t *frame = bitbyte_foundation_timer_create();
  bitbyte_foundation_timer_t *wall = bitbyte_foundation_timer_create();
  while (true) {
    bitbyte_butane_time_step_policy_t *time_step_policy = application->time_step_policy;
    bitbyte_butane_time_step_policy_update(time_step_policy, frame, wall);
    for (size_t step = 0; step < time_step_policy->steps; ++step)
      application->update(application, time_step_policy->delta_time_per_step);
    application->render(application);
    // BUG: This may result in all frames taking "zero" microseconds, due to
    // Due to inadequate timing resolution.
    bitbyte_foundation_timer_reset(frame);
  }
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_application_quit(
  bitbyte_butane_application_t *application)
{
  bitbyte_butane_assert_debug(application != NULL);
  application->shutdown(application);
  ::exit(EXIT_SUCCESS);
}

//===----------------------------------------------------------------------===//

const char *
bitbyte_butane_application_platform()
{
#if BITBYTE_FOUNDATION_PLATFORM == __BITBYTE_FOUNDATION_PLATFORM_WINDOWS__
  return "windows";
#elif BITBYTE_FOUNDATION_PLATFORM == __BITBYTE_FOUNDATION_PLATFORM_MACOSX__
  return "macosx";
#elif BITBYTE_FOUNDATION_PLATFORM == __BITBYTE_FOUNDATION_PLATFORM_LINUX__
  return "linux";
#elif BITBYTE_FOUNDATION_PLATFORM == __BITBYTE_FOUNDATION_PLATFORM_IOS__
  return "ios";
#elif BITBYTE_FOUNDATION_PLATFORM == __BITBYTE_FOUNDATION_PLATFORM_ANDROID__
  return "android";
#endif
}

//===----------------------------------------------------------------------===//

const char *
bitbyte_butane_application_build()
{
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_DEBUG
  return "debug";
#elif BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_DEVELOPMENT
  return "development";
#elif BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_RELEASE
  return "release";
#endif
}

//===----------------------------------------------------------------------===//

#ifdef __cplusplus
}
#endif // __cplusplus

//============================================================================//

#ifdef __cplusplus

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//

Application::Application()
  : __self__(this)
{
  struct Proxy {
    static bool initialize(::bitbyte_butane_application_t *application) {
      return (*(Application **)(((uintptr_t)application) + sizeof(::bitbyte_butane_application_t)))->initialize();
    }

    static void update(::bitbyte_butane_application_t *application, const float delta_time) {
      (*(Application **)(((uintptr_t)application) + sizeof(::bitbyte_butane_application_t)))->update(delta_time);
    }

    static void render(const ::bitbyte_butane_application_t *application) {
      (*(Application **)(((uintptr_t)application) + sizeof(::bitbyte_butane_application_t)))->render();
    }

    static void shutdown(::bitbyte_butane_application_t *application) {
      (*(Application **)(((uintptr_t)application) + sizeof(::bitbyte_butane_application_t)))->shutdown();
    }
  };

  __proxy__.initialize = &Proxy::initialize;
  __proxy__.update = &Proxy::update;
  __proxy__.render = &Proxy::render;
  __proxy__.shutdown = &Proxy::shutdown;
}

Application::~Application()
{
}

//===----------------------------------------------------------------------===//

bool Application::initialize(void)
{
  bitbyte_butane_assertf_always("Application::initialize() is not overridden!");
  return false;
}

void Application::update(const float delta_time)
{
  bitbyte_butane_assertf_always("Application::update() is not overridden!");
}

void Application::render(void) const
{
  bitbyte_butane_assertf_always("Application::render() is not overridden!");
}

void Application::shutdown(void)
{
  bitbyte_butane_assertf_always("Application::shutdown() is not overridden!");
}

//===----------------------------------------------------------------------===//

void Application::start(void)
{
  ::bitbyte_butane_application_start(
    (::bitbyte_butane_application_t *)&__proxy__);
}

void Application::run(void)
{
  ::bitbyte_butane_application_run(
    (::bitbyte_butane_application_t *)&__proxy__);
}

void Application::quit(void)
{
  ::bitbyte_butane_application_quit(
    (::bitbyte_butane_application_t *)&__proxy__);
}

//===----------------------------------------------------------------------===//

const char *Application::platform()
{
  return ::bitbyte_butane_application_platform();
}

const char *Application::build()
{
  return ::bitbyte_butane_application_build();
}

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//
