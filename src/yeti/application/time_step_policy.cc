//===-- yeti/application/time_step_policy.cc ------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/application/time_step_policy.h"

// For rounding.
#include <math.h>

namespace yeti {

TimeStepPolicy::TimeStepPolicy() {
  desc_.type = TimeStepPolicy::UNKNOWN;
  steps_ = 0;
  delta_time_per_step_ = 0.f;
}

TimeStepPolicy::~TimeStepPolicy() {
}

TimeStepPolicy *TimeStepPolicy::create(const TimeStepPolicy::Description &desc) {
  // TODO(mtwilliams): Perform more thorough validation.
  yeti_assert_development(desc.type != TimeStepPolicy::UNKNOWN);

  TimeStepPolicy *time_step_policy =
    YETI_NEW(TimeStepPolicy, core::global_heap_allocator());

  time_step_policy->desc_ = desc;

  core::memory::zero((void *)&time_step_policy->state_, sizeof(TimeStepPolicy::State));

  // TODO(mtwilliams): Implement the smoothed time-step-policy.
  yeti_assert_release(desc.type != TimeStepPolicy::SMOOTHED);

  return time_step_policy;
}

void TimeStepPolicy::destroy() {
  YETI_DELETE(TimeStepPolicy, core::global_heap_allocator(), this);
}

void TimeStepPolicy::update(const core::Timer &frame,
                            const core::Timer &wall) {
  switch (desc_.type) {
    case TimeStepPolicy::VARIABLE: {
      steps_ = 1;
      delta_time_per_step_ = (f32)frame.usecs() / 1000000.f;
    } break;

    case TimeStepPolicy::FIXED: {
      state_.fixed.accumulated += (f32)frame.usecs() / 1000000.f;
      steps_ = lroundf(state_.fixed.accumulated / desc_.config.fixed.delta_time_per_step);
      state_.fixed.accumulated -= steps_ * desc_.config.fixed.delta_time_per_step;
      delta_time_per_step_ = desc_.config.fixed.delta_time_per_step;
    } break;

    case TimeStepPolicy::SMOOTHED: {
      YETI_TRAP();
    } break;
  }
}

} // yeti
