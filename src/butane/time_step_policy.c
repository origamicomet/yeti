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
 #  include <butane/time_step_policy.h>
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_time_step_policy_t):                                            */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */

size_t butane_time_step_policy_num_of_steps(
  const butane_time_step_policy_t *time_step_policy)
{
  butane_assert(debug, time_step_policy != NULL);
  return time_step_policy->num_of_steps_;
}

float butane_time_step_policy_delta_time_per_step(
  const butane_time_step_policy_t *time_step_policy)
{
  butane_assert(debug, time_step_policy != NULL);
  return time_step_policy->delta_time_per_step_;
}

/* ========================================================================== */

typedef struct {
  butane_time_step_policy_t tsp;
} butane_variable_time_step_policy_t;

static void butane_variable_time_step_policy__update(
  butane_time_step_policy_t *time_step_policy,
  const fnd_monotonic_clock_t *wall,
  const fnd_monotonic_clock_t *frame)
{
  butane_assert(paranoid, time_step_policy != NULL);
  butane_variable_time_step_policy_t *tsp =
    ((butane_variable_time_step_policy_t *)time_step_policy);
  tsp->tsp.num_of_steps_ = 1;
  tsp->tsp.delta_time_per_step_ =
    ((float)fnd_monotonic_clock_nsecs(frame) / 1000000000.0f);
}

static void butane_variable_time_step_policy__destroy(
  butane_time_step_policy_t *time_step_policy)
{
  butane_assert(paranoid, time_step_policy != NULL);
  fnd_allocator_free(butane_heap(), (void *)time_step_policy);
}

butane_time_step_policy_t *butane_time_step_policy_variable(void) {
  /* TODO(mtwilliams): Use fnd_proxy_allocator_t. */
  butane_variable_time_step_policy_t *tsp = (butane_variable_time_step_policy_t *)
    fnd_allocator_alloc(
      butane_heap(),
      sizeof(butane_variable_time_step_policy_t),
      fnd_alignof(butane_variable_time_step_policy_t));
  tsp->tsp.update = &butane_variable_time_step_policy__update;
  tsp->tsp.destroy = &butane_variable_time_step_policy__destroy;
  tsp->tsp.num_of_steps_ = 0;
  tsp->tsp.delta_time_per_step_ = 0.0f;
  return &tsp->tsp;
}

/* ========================================================================== */

void butane_time_step_policy_update(
  butane_time_step_policy_t *time_step_policy,
  const fnd_monotonic_clock_t *wall,
  const fnd_monotonic_clock_t *frame)
{
  butane_assert(debug, time_step_policy != NULL);
  butane_assert(debug, time_step_policy->update != NULL);
  butane_assert(debug, wall != NULL);
  butane_assert(debug, frame != NULL);
  time_step_policy->update(time_step_policy, wall, frame);
}

void butane_time_step_policy_destroy(
  butane_time_step_policy_t *time_step_policy)
{
  butane_assert(debug, time_step_policy != NULL);
  butane_assert(debug, time_step_policy->destroy != NULL);
  time_step_policy->destroy(time_step_policy);
}

/* ========================================================================== */

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::TimeStepPolicy):                                             */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /* ======================================================================== */

  // TimeStepPolicy::TimeStepPolicy() {
  // }
  //
  // TimeStepPolicy::TimeStepPolicy(const TimeStepPolicy &) {
  // }
  //
  // TimeStepPolicy &TimeStepPolicy::operator= (const TimeStepPolicy &) {
  // }
  //
  // TimeStepPolicy::~TimeStepPolicy() {
  // }

  /* ======================================================================== */

  TimeStepPolicy *TimeStepPolicy::recover_(::butane_time_step_policy_t *tsp) {
    butane_assert(development, tsp != NULL);
    return ((TimeStepPolicy *)tsp);
  }

  ::butane_time_step_policy_t *TimeStepPolicy::lose_() {
    return ((::butane_time_step_policy_t *)this);
  }

  const TimeStepPolicy *TimeStepPolicy::recover_(const ::butane_time_step_policy_t *tsp) {
    butane_assert(development, tsp != NULL);
    return ((const TimeStepPolicy *)tsp);
  }

  const ::butane_time_step_policy_t *TimeStepPolicy::lose_() const {
    return ((const ::butane_time_step_policy_t *)this);
  }

  /* ======================================================================== */

  size_t TimeStepPolicy::num_of_steps() const {
    return butane_time_step_policy_num_of_steps(this->lose_());
  }

  float TimeStepPolicy::delta_time_per_step() const {
    return butane_time_step_policy_delta_time_per_step(this->lose_());
  }

  /* ======================================================================== */

  TimeStepPolicy *TimeStepPolicy::variable() {
    return ((TimeStepPolicy *)butane_time_step_policy_variable());
  }

  void TimeStepPolicy::update(
    const foundation::clock::Monotonic &wall,
    const foundation::clock::Monotonic &frame)
  {
    butane_time_step_policy_update(this->lose_(), wall.lose_(), frame.lose_());
  }

  void TimeStepPolicy::destroy() {
    butane_time_step_policy_destroy(this->lose_());
  }

  /* ======================================================================== */
} /* butane */
#endif

/* ========================================================================== */
