// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TIME_STEP_POLICY_H_
#define _BUTANE_TIME_STEP_POLICY_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT TimeStepPolicy final {
    public:
      enum Policy {
        VARIABLE = 1,
        FIXED    = 2,
        SMOOTHED = 3
      };

    public:
      TimeStepPolicy();
      ~TimeStepPolicy();

    public:
      static TimeStepPolicy variable();

      static TimeStepPolicy fixed(
        const float target_delta_time );

      static TimeStepPolicy smoothed(
        const size_t history,
        const size_t outliers,
        const float rate );

    public:
      void frame(
        const float delta_time );

    public:
      FOUNDATION_INLINE size_t num_of_steps() const
      { return _num_of_steps; }

      FOUNDATION_INLINE float delta_time_per_step() const
      { return _delta_time_per_step; }

    public:
      FOUNDATION_INLINE Policy policy() const
      { return _policy; }

    private:
      Policy _policy;
      union {
        struct {
        } variable;
        struct {
          float target_delta_time;
        } fixed;
        struct {
          size_t history;
          size_t outliers;
          float rate;
        } smoothed;
      } _settings;
      union {
        struct {
        } variable;
        struct {
          float debt;
        } fixed;
        struct {
          size_t saturation;
          float history[32];
        } smoothed;
      } _data;
      size_t _num_of_steps;
      float _delta_time_per_step;
  };
} // butane

#endif // _BUTANE_TIME_STEP_POLICY_H_
