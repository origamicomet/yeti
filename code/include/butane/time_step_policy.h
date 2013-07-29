// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TIME_STEP_POLICY_H_
#define _BUTANE_TIME_STEP_POLICY_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT TimeStepPolicy final {
    private:
      enum Policy {
        VARIABLE = 1,
      };

    public:
      TimeStepPolicy();
      ~TimeStepPolicy();

    public:
      static TimeStepPolicy variable();

    public:
      void frame(
        const float delta_time );

    public:
      FOUNDATION_INLINE size_t num_of_steps() const
      { return _num_of_steps; }

      FOUNDATION_INLINE float delta_time_per_step() const
      { return _delta_time_per_step; }

    private:
      Policy _policy;
      union {
        struct {
        } variable;
      } _settings;
      size_t _num_of_steps;
      float _delta_time_per_step;
  };
} // butane

#endif // _BUTANE_TIME_STEP_POLICY_H_
