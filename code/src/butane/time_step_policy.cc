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

  TimeStepPolicy TimeStepPolicy::smoothed(
    const size_t history,
    const size_t outliers,
    const float rate )
  {
    assert(history > 0);
    assert(history <= 32);
    assert((outliers * 2) < history);
    assert(rate > 0.0f);

    TimeStepPolicy tsp;
    tsp._policy = SMOOTHED;
    tsp._settings.smoothed.history = history;
    tsp._settings.smoothed.outliers = outliers;
    tsp._settings.smoothed.rate = rate;
    tsp._data.smoothed.saturation = 0;
    zero((void*)&tsp._data.smoothed.history[0], 32 * sizeof(float));
    return tsp;
  }

  TimeStepPolicy TimeStepPolicy::smoothed_with_debt_payback(
    const size_t history,
    const size_t outliers,
    const float rate,
    const float payback_rate )
  {
    assert(payback_rate > 0.0f);

    TimeStepPolicy tsp = TimeStepPolicy::smoothed(history, outliers, rate);
    tsp._policy = SMOOTHED_WITH_DEBT_PAYBACK;
    tsp._settings.smoothed.payback_rate = payback_rate;
    tsp._data.smoothed.debt = 0.0f;
    return tsp;
  }

  void TimeStepPolicy::frame(
    const float delta_time )
  {
    switch (_policy) {
      case VARIABLE: {
        _num_of_steps = 1;
        _delta_time_per_step = delta_time;
      } break;
      case FIXED: {
        _data.fixed.debt += delta_time;
        _num_of_steps = (size_t)(_data.fixed.debt / _settings.fixed.target_delta_time);
        _delta_time_per_step = _settings.fixed.target_delta_time;
        _data.fixed.debt = fmod(_data.fixed.debt, _settings.fixed.target_delta_time);
      } break;
      case SMOOTHED:
      case SMOOTHED_WITH_DEBT_PAYBACK: {
        float* sorted = (float*)alloca(_data.smoothed.saturation * sizeof(float));
        copy(
          (void*)sorted,
          (const void*)&_data.smoothed.history[32 - _data.smoothed.saturation],
          (_data.smoothed.saturation) * sizeof(float));
        sort(sorted, _data.smoothed.saturation);
        if (_data.smoothed.saturation >= (_settings.smoothed.outliers * 2 + 1)) {
          const float avg = arithmetic_mean(
            &sorted[_settings.smoothed.outliers],
            _data.smoothed.saturation - (_settings.smoothed.outliers * 2));
          const float last_frame =
            _data.smoothed.history[_data.smoothed.saturation - 1];
          const float smoothed =
            _settings.smoothed.rate*avg + (1-_settings.smoothed.rate) * last_frame;
          _num_of_steps = 1;
          _delta_time_per_step = smoothed;
        } else {
          _num_of_steps = 1;
          _delta_time_per_step = delta_time; }
        copy_safe(
          (void*)&_data.smoothed.history[0],
          (const void*)&_data.smoothed.history[1],
          (_settings.smoothed.history - 1) * sizeof(float));
        _data.smoothed.history[31] = delta_time;
        if (_policy == SMOOTHED_WITH_DEBT_PAYBACK) {
          const float payback = _data.smoothed.debt * _settings.smoothed.payback_rate;
          _delta_time_per_step += payback;
          _data.smoothed.debt -= payback; }
      } break;
      default:
        __builtin_unreachable();
    }
  }
} // butane
