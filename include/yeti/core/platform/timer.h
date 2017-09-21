//===-- yeti/core/platform/timer.h ----------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief High resolution timing.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_TIMER_H_
#define _YETI_CORE_PLATFORM_TIMER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// A high-resolution monotonically increasing timer.
class YETI_PUBLIC Timer {
 YETI_DISALLOW_COPYING(Timer)

 public:
  Timer();
  ~Timer();

 public:
  /// \brief Resets the timer.
  void reset();

 public:
  /// \brief Returns the number of seconds elapsed since the last reset.
  u64 secs() const;

  /// \brief Returns the number of milliseconds elapsed since the last reset.
  u64 msecs() const;

  /// \brief Returns the number of microseconds elapsed since the last reset.
  u64 usecs() const;

  /// \brief Returns the number of nanoseconds elapsed since the last reset.
  /// \warning Nanosecond resolution is not guaranteed on some platforms.
  u64 nsecs() const;

 private:
  /// \internal Ticks since last reset.
  u64 elapsed() const;

 private:
  u64 frequency_;
  u64 epoch_;
};

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_TIMER_H_
