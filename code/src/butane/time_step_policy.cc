// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/time_step_policy.h>

#include <math.h>

namespace butane {
  TimeStepPolicy::TimeStepPolicy()
    : _policy(VARIABLE)
  {}

  TimeStepPolicy::~TimeStepPolicy()
  {}

  TimeStepPolicy TimeStepPolicy::variable()
  {
    TimeStepPolicy tsp;
    tsp._policy = VARIABLE;
    return tsp;
  }

  TimeStepPolicy TimeStepPolicy::fixed(
    const float target_delta_time )
  {
    TimeStepPolicy tsp;
    tsp._policy = FIXED;
    tsp._settings.fixed.target_delta_time = target_delta_time;
    tsp._data.fixed.debt = 0.0f;
    return tsp;
  }

  void TimeStepPolicy::frame(
    const float delta_time )
  {
    switch (_policy) {
      case VARIABLE:
        _num_of_steps = 1;
        _delta_time_per_step = delta_time;
        break;
      case FIXED:
        _data.fixed.debt += delta_time;
        _num_of_steps = (size_t)(_data.fixed.debt / _settings.fixed.target_delta_time);
        _delta_time_per_step = _settings.fixed.target_delta_time;
        _data.fixed.debt = fmod(_data.fixed.debt, _settings.fixed.target_delta_time);
        break;
      default:
        __builtin_unreachable();
    }
  }
} // butane
