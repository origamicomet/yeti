// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/time_step_policy.h>

namespace butane {
  TimeStepPolicy::TimeStepPolicy()
    : _policy(VARIABLE)
  {}

  TimeStepPolicy::~TimeStepPolicy()
  {}

  TimeStepPolicy TimeStepPolicy::variable()
  { return TimeStepPolicy(); }

  void TimeStepPolicy::frame(
    const float delta_time,
    size_t& num_of_steps,
    float& delta_time_per_step )
  {
    switch (_policy) {
      case VARIABLE:
        num_of_steps = 1;
        delta_time_per_step = delta_time;
        break;
      default:
        __builtin_unreachable();
    }
  }
} // butane
