//===-- yeti/foundation/high_res_timer.h ------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_HIGH_RES_TIMER_H_
#define _YETI_FOUNDATION_HIGH_RES_TIMER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// A high-resolution monotonic timer.
class YETI_PUBLIC HighResolutionTimer {
 YETI_DISALLOW_CONSTRUCTION(HighResolutionTimer);
 YETI_DISALLOW_COPYING(HighResolutionTimer);

 public:
  /// \brief Creates a high-resolution monotonic timer.
  static HighResolutionTimer *create();

  /// \brief Destroys this timer.
  void destroy();

 public:
  /// \brief Resets this timer.
  void reset();

 public:
  /// \brief Returns the number of seconds elapsed since the last reset.
  u64 secs() const;

  /// \brief Returns the number of miliseconds elapsed since the last reset.
  u64 msecs() const;

  /// \brief Returns the number of microseconds elapsed since the last reset.
  u64 usecs() const;

  /// \brief Returns the number of nanoseconds elapsed since the last reset.
  ///
  /// \warning Nanosecond resolution is not guaranteed on some platforms, namely
  /// Microsoft Windows as QueryPerformanceCounter usually takes the worst code
  /// path possible.
  ///
  u64 nsecs() const;

 private:
  struct Implementation;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_HIGH_RES_TIMER_H_
