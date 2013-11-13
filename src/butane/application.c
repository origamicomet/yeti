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

void butane_application_update(
  butane_application_t *app,
  const float delta_time)
{
  butane_assert(debug, app != NULL);
  butane_assert(debug, app->update != NULL);
  app->update(app, delta_time);
}

void butane_application_render(
  const butane_application_t *app)
{
  butane_assert(debug, app != NULL);
  butane_assert(debug, app->render != NULL);
  app->render(app);
}

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::Application):                                                */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  Application::Application() {
    _.update = &Application::update_;
    _.render = &Application::render_;
    this_ = this;
  }

  Application::~Application() {
  }

  /* ======================================================================== */

  void Application::update(const float delta_time) {
    butane_assertf_(0, "butane::Application::update(delta_time) not overridden!");
  }

  void Application::render() const {
    butane_assertf_(0, "butane::Application::render() not overridden!");
  }

  /* ======================================================================== */

  Application *Application::recover_(::butane_application_t *app) {
    return *((Application **)
      ((uintptr_t)app) +
      (offsetof(Application, this_) - offsetof(Application, _)));
  }

  const Application *Application::recover_(const ::butane_application_t *app) {
    return *((const Application **)
      ((uintptr_t)app) +
      (offsetof(Application, this_) - offsetof(Application, _)));
  }

  /* ======================================================================== */

  void Application::update_(::butane_application_t *app, const float delta_time) {
    recover_(app)->update(delta_time);
  }

  void Application::render_(const ::butane_application_t *app) {
    recover_(app)->render();
  }
} /* butane */
#endif

/* ========================================================================== */
