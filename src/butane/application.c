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

/* ========================================================================== */
/*  C (butane_application_t):                                                 */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

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
  app->shutdown(app);
}

/* =========================================================================== */

void butane_application_run(
  butane_application_t *app)
{
  butane_assert(debug, app != NULL);

  if (!butane_application_initialize(app))
    return;

  while (true) {
    /* TODO(mtwilliams): Implement time-step policies. */
    butane_application_update(app, 1.0f / 60.0f);
    butane_application_render(app);
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
