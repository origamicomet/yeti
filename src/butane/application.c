/* ========================================================================== */
/*                                                                            */
/* This file is part of Butane.                                               */
/*                                                                            */
/* Author(s):                                                                 */
/*                                                                            */
/*   Michael Williams <devbug@bitbyte.ca>                                     */
/*                                                                            */
/* See LICENSE.md for licensing information.                                  */
/*                                                                            */
/* ========================================================================== */
 #  include <butane/application.h>
/* ========================================================================== */

#include <butane/time_step_policy.h>

/* ========================================================================== */
/*  C (butane_application_t):                                                 */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */

butane_time_step_policy_t *butane_application_time_step_policy(
  const butane_application_t *app)
{
  butane_assert(debug, app != NULL);
  return app->time_step_policy_;
}

void butane_application_set_time_step_policy(
  butane_application_t *app,
  butane_time_step_policy_t *time_step_policy)
{
  butane_assert(debug, app != NULL);
  butane_assert(development, time_step_policy != NULL);
  // TENTATIVE(mtwilliams): Don't automatically destroy?
  if (app->time_step_policy_)
    butane_time_step_policy_destroy(app->time_step_policy_);
  app->time_step_policy_ = time_step_policy;
}

/* ========================================================================== */

bool butane_application_initialize(
  butane_application_t *app)
{
  butane_assert(debug, app != NULL);
  butane_assert(debug, app->initialize != NULL);
  return app->initialize(app);
}

void butane_application_update(
  butane_application_t *app,
  const float delta_time)
{
  butane_assert(debug, app != NULL);
  butane_assert(debug, app->update != NULL);
  // TODO(mtwilliams): Support negative delta-times?
  butane_assert(development, delta_time > 0.0f);
  app->update(app, delta_time);
}

void butane_application_render(
  const butane_application_t *app)
{
  butane_assert(debug, app != NULL);
  butane_assert(debug, app->render != NULL);
  app->render(app);
}

void butane_application_shutdown(
  butane_application_t *app)
{
  butane_assert(debug, app != NULL);
  butane_assert(debug, app->shutdown != NULL);
  butane_time_step_policy_destroy(app->time_step_policy_);
  app->shutdown(app);
}

/* =========================================================================== */

void butane_application_run(
  butane_application_t *app)
{
  butane_assert(debug, app != NULL);
  if (!butane_application_initialize(app))
    return;
  fnd_monotonic_clock_t *wall = fnd_monotonic_clock_create();
  fnd_monotonic_clock_t *frame = fnd_monotonic_clock_create();
  while (true) {
    butane_time_step_policy_t *tsp = butane_application_time_step_policy(app);
    butane_time_step_policy_update(tsp, wall, frame);
    const size_t num_of_steps = butane_time_step_policy_num_of_steps(tsp);
    const float delta_time_per_step = butane_time_step_policy_delta_time_per_step(tsp);
    if (delta_time_per_step <= 0.0f)
      continue;
    for (size_t step = 0; step < num_of_steps; ++step)
      butane_application_update(app, delta_time_per_step);
    butane_application_render(app);
    fnd_monotonic_clock_reset(frame);
  }
}

/* =========================================================================== */

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::Application):                                                */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /* ======================================================================== */

  Application::Application() {
    _.initialize = &Application::initialize_;
    _.update = &Application::update_;
    _.render = &Application::render_;
    _.shutdown = &Application::shutdown_;
    this_ = this;
  }

  Application::~Application() {
  }

  /* ======================================================================== */

  Application *Application::recover_(::butane_application_t *app) {
    butane_assert(development, app != NULL);
    static const size_t offset =
      (offsetof(Application, this_) - offsetof(Application, _));
    return *((Application **)(((uintptr_t)app) + offset));
  }

  ::butane_application_t *Application::lose_() {
    return ((::butane_application_t *)&_);
  }

  const Application *Application::recover_(const ::butane_application_t *app) {
    butane_assert(development, app != NULL);
    static const size_t offset =
      (offsetof(Application, this_) - offsetof(Application, _));
    return *((const Application **)(((uintptr_t)app) + offset));
  }

  const ::butane_application_t *Application::lose_() const {
    return ((const ::butane_application_t *)&_);
  }

  /* ======================================================================== */

  TimeStepPolicy *Application::time_step_policy() const {
    return TimeStepPolicy::recover_(butane_application_time_step_policy(this->lose_()));
  }

  void Application::set_time_step_policy(TimeStepPolicy *time_step_policy) {
    butane_assert(debug, time_step_policy != NULL);
    butane_application_set_time_step_policy(this->lose_(), time_step_policy->lose_());
  }

  /* ======================================================================== */

  bool Application::initialize() {
    butane_assertf_(0, "butane::Application::initialize() not overridden!");
    return false;
  }

  void Application::update(const float delta_time) {
    butane_assertf_(0, "butane::Application::update(delta_time) not overridden!");
  }

  void Application::render() const {
    butane_assertf_(0, "butane::Application::render() not overridden!");
  }

  void Application::shutdown() {
    butane_assertf_(0, "butane::Application::shutdown() not overridden!");
  }

  /* ======================================================================== */

  bool Application::initialize_(::butane_application_t *app) {
    return recover_(app)->initialize();
  }

  void Application::update_(::butane_application_t *app, const float delta_time) {
    recover_(app)->update(delta_time);
  }

  void Application::render_(const ::butane_application_t *app) {
    recover_(app)->render();
  }

  void Application::shutdown_(::butane_application_t *app) {
    recover_(app)->render();
  }

  /* ======================================================================== */

  void Application::run() {
    butane_application_run(this->lose_());
  }

  /* ======================================================================== */
} /* butane */
#endif

/* ========================================================================== */
