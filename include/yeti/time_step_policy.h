//===-- yeti/time_step_policy.h ---------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_TIME_STEP_POLICY_H_
#define _YETI_TIME_STEP_POLICY_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

/// ...
class YETI_PUBLIC TimeStepPolicy {
 YETI_DISALLOW_COPYING(TimeStepPolicy);

 public:
  enum Type {
    /// Unknown.
    UNKNOWN  = 0,
    /// Variable.
    VARIABLE = 1,
    /// Fixed.
    FIXED    = 2,
    /// Smoothed.
    SMOOTHED = 3
  };

 public:
  struct Description {
    Type type;
    union {
      struct {
      } variable;

      struct {
        f32 delta_time_per_step;
      } fixed;

      struct {
        u32 history;
        u32 outliers;
        f32 momentum;
        bool payback;
      } smoothed;
    } config;
  };

 private:
  struct State {
    union {
      struct {
      } variable;

      struct {
        f32 accumulated;
      } fixed;

      struct {
        f32 *history;
        f32 debt;
      } smoothed;
    };
  };

 private:
  TimeStepPolicy();
  ~TimeStepPolicy();

 public:
  static TimeStepPolicy *create(const TimeStepPolicy::Description &desc);
  void destroy();

 public:
  // TODO(mtwilliams): Document this interface.
  void update(const foundation::HighResolutionTimer *frame,
              const foundation::HighResolutionTimer *wall);

 public:
  const TimeStepPolicy::Description &desc() const;

 public:
  // TODO(mtwilliams): Document this interface.
  u32 steps() const;
  f32 delta_time_per_step() const;

 private:
  Description desc_;
  State state_;
  u32 steps_;
  f32 delta_time_per_step_;
};

YETI_INLINE const TimeStepPolicy::Description &TimeStepPolicy::desc() const {
  return desc_;
}

YETI_INLINE u32 TimeStepPolicy::steps() const {
  return steps_;
}

YETI_INLINE f32 TimeStepPolicy::delta_time_per_step() const {
  return delta_time_per_step_;
}

} // yeti

#endif // _YETI_TIME_STEP_POLICY_H_
