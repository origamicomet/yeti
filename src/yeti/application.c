#if 0

/*===-- yeti/application.c ----------------------------------*- mode: C -*-===*\
|*                                                                            *|
|*                             __ __     _   _                                *|
|*                            |  |  |___| |_|_|                               *|
|*                            |_   _| -_|  _| |                               *|
|*                              |_| |___|_| |_|                               *|
|*                                                                            *|
|*       This file is distributed under the terms described in LICENSE.       *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#include "yeti/application.h"

#include <stdlib.h>

YETI_BEGIN_EXTERN_C /* { */

static yeti_bool_t _assert_on_init(struct yeti_application *app);
static void _assert_on_shutdown(struct yeti_application *app);
static void _assert_on_update(struct yeti_application *app,
                              const yeti_real32_t delta_time);
static void _assert_on_render(struct yeti_application const *app);

static void _run(struct yeti_application *app);

yeti_bool_t yeti_application_init(struct yeti_application *app) {
  yeti_assert_debug(app != NULL);
  app->init = &_assert_on_init;
  app->shutdown = &_assert_on_shutdown;
  app->update = &_assert_on_update;
  app->render = &_assert_on_render;
}

void yeti_application_shutdown(struct yeti_application *app) {
  yeti_assert_debug(app != NULL);
}

void yeti_application_update(struct yeti_application *app,
                             const yeti_real32_t delta_time) {
  yeti_assert_debug(app != NULL);
  yeti_assert_debug(delta_time >= 0.f);
}

void yeti_application_render(struct yeti_application const *app) {
  yeti_assert_debug(app != NULL);
}

void yeti_application_pause(struct yeti_application *app) {
  yeti_assert_debug(app != NULL);
  /* TODO(mtwilliams): Implement pausing. */
  yeti_assert_always("Not implemented, yet!");
}

void yeti_application_unpause(struct yeti_application *app) {
  /* TODO(mtwilliams): Implement unpausing. */
  yeti_assert_debug(app != NULL);
}

void yeti_application_quit(struct yeti_application *app) {
  yeti_assert_debug(app != NULL);
  app->shutdown(app);
  exit(EXIT_SUCCESS);
}

void yeti_application_init_and_run(struct yeti_application *app) {
  yeti_assert_debug(app != NULL);
  const yeti_bool successfully_initialized = app->init(app);
  if (!successfully_initialized)
    yeti_assert_always("Application initialization failed!");
#if 0
  if (app->time_step_policy == NULL) {
    yeti_log(YETI_LOG_LEVEL_WARNING,
             "No time-step policy specified!\n"
             " => defaulting to `variable'\n");
    /* app->time_step_policy = */ {
      yeti_time_step_policy_opts_t opts;
      opts.type = YETI_TIME_STEP_POLICY_VARIABLE;
      app->time_step_policy = yeti_time_step_policy_create(&opts);
    }
  }
  if (app->window == NULL) {
    yeti_log(YETI_LOG_LEVEL_WARNING,
             "No window created!\n"
             " => defaulting to 1280x720\n");
    /* app->window = */ {
      yeti_window_opts_t opts;
      opts.title = "Untitled";
      opts.width = 1280;
      opts.height = 720;
      app->window = yeti_window_open(&opts);
    }
  }
  yeti_window_show(app->window);
#endif
  _run(app);
}

static yeti_bool_t _assert_on_init(struct yeti_application *app) {
  YETI_UNUSED(app);
  yeti_assert_always("Application's initialization callback was never specified!");
}

static void _assert_on_shutdown(struct yeti_application *app) {
  YETI_UNUSED(app);
  yeti_assert_always("Application's shutdown callback was never specified!");
}

static void _assert_on_update(struct yeti_application *app,
                              const yeti_real32_t delta_time) {
  YETI_UNUSED(app);
  yeti_assert_always("Application's update callback was never specified!");
}

static void _assert_on_render(struct yeti_application const *app) {
  YETI_UNUSED(app);
  yeti_assert_always("Application's render callback was never specified!");
}

static void _run(struct yeti_application *app) {
  yeti_assert_debug(app != NULL);
  /* TODO(mtwilliams): Move this into a task? */
#if 0
  yeti_timer_t *frame = yeti_timer_create();
  yeti_timer_t *wall = yeti_timer_create();
  for (;;) {
    yeti_time_step_policy_t *time_step_policy = app->time_step_policy;
    yeti_assert_debug(time_step_policy != NULL);
    yeti_time_step_policy_update(time_step_policy, frame, wall);

    yeti_assert_debug(app->window != NULL);
    yeti_window_update(app->window);

    /* It's important that these are pulled out prior to updating, just in
     * case the time-step policy is changed during a step.
     */
    const yeti_size_t num_steps = time_step_policy->num_steps;
    const yeti_real32_t delta_time_per_step = time_step_policy->delta_time_per_step;

    for (yeti_size_t step = 0; step < num_steps; ++step)
      app->update(app, delta_time_per_step);

    /* BUG: Due to inadequate timing resolution, this may result in all
     * frames taking `zero' microseconds unless enough work is performed
     * every frame.
     */
    yeti_timer_reset(frame);
  }
#endif
}

YETI_END_EXTERN_C /* } */

/*============================================================================*/

#endif
