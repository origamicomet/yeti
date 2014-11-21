//===-- bitbyte/butane/time_step_policy.c -----------------------*- C++ -*-===//
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

#include "bitbyte/butane/time_step_policy.h"

//===----------------------------------------------------------------------===//

#include <stdlib.h>

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

typedef bitbyte_butane_time_step_policy_t bitbyte_butane_time_step_policy_variable_t;
static bitbyte_butane_time_step_policy_variable_t *variable(void) {
  return (bitbyte_butane_time_step_policy_variable_t *)
    malloc(sizeof(bitbyte_butane_time_step_policy_variable_t));
}

bitbyte_butane_time_step_policy_t *
bitbyte_butane_time_step_policy_create(
  const bitbyte_butane_time_step_policy_opts_t *opts)
{
  bitbyte_butane_assert_debug(opts != NULL);
  bitbyte_butane_assert_debug(opts->type != BITBYTE_BUTANE_TIME_STEP_POLICY_UNKNOWN);

  switch (opts->type) {
    case BITBYTE_BUTANE_TIME_STEP_POLICY_VARIABLE: {
      bitbyte_butane_time_step_policy_variable_t *time_step_policy = variable();
      memcpy((void *)&time_step_policy->opts, (const void *)opts, sizeof(bitbyte_butane_time_step_policy_opts_t));
      time_step_policy->steps = 0;
      time_step_policy->delta_time_per_step = 0.f;
      return time_step_policy;
    } break;
    default: {
      bitbyte_butane_assertf(0, "Unknown or unsupported time-step policy!");
    } break;
  }

  return NULL;
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_time_step_policy_update(
  bitbyte_butane_time_step_policy_t *time_step_policy_,
  const bitbyte_foundation_timer_t *frame,
  const bitbyte_foundation_timer_t *wall)
{
  bitbyte_butane_assert_debug(time_step_policy_ != NULL);
  bitbyte_butane_assert_debug(frame != NULL);
  bitbyte_butane_assert_debug(wall != NULL);

  switch (time_step_policy_->opts.type) {
    case BITBYTE_BUTANE_TIME_STEP_POLICY_VARIABLE: {
      bitbyte_butane_time_step_policy_variable_t *time_step_policy =
        (bitbyte_butane_time_step_policy_variable_t *)time_step_policy_;
      time_step_policy->steps = 1;
      time_step_policy->delta_time_per_step = bitbyte_foundation_timer_msecs(frame) / 1000000.f;
    } break;
    default: {
      // Unreachable.
    } break;
  }
}

//===----------------------------------------------------------------------===//

void
bitbyte_butane_time_step_policy_destroy(
  bitbyte_butane_time_step_policy_t *time_step_policy)
{
  bitbyte_butane_assert_debug(time_step_policy != NULL);
  free((void *)time_step_policy);
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

TimeStepPolicy *TimeStepPolicy::create(
  const TimeStepPolicy::Options &options)
{
  return (TimeStepPolicy *)::bitbyte_butane_time_step_policy_create(
    (const ::bitbyte_butane_time_step_policy_opts_t *)&options);
}

void TimeStepPolicy::update(
  const bitbyte::foundation::Timer &frame,
  const bitbyte::foundation::Timer &wall)
{
  ::bitbyte_butane_time_step_policy_update(
    (::bitbyte_butane_time_step_policy_t *)this,
    (const ::bitbyte_foundation_timer_t *)&frame,
    (const ::bitbyte_foundation_timer_t *)&wall);
}

void TimeStepPolicy::destroy(void)
{
  ::bitbyte_butane_time_step_policy_destroy(
    (::bitbyte_butane_time_step_policy_t *)this);
}

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//
