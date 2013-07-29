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
        const float delta_time,
        size_t& num_of_steps,
        float& delta_time_per_step );

    private:
      Policy _policy;
      union {
        struct {
        } variable;
      } _settings;
  };
} // butane

#endif // _BUTANE_TIME_STEP_POLICY_H_
