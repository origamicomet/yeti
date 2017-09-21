//===-- yeti/core/platform/event.h ----------------------*- mode: C++11 -*-===//
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
/// \brief Lightweight signaling primitive.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_EVENT_H_
#define _YETI_CORE_PLATFORM_EVENT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// A lightweight signaling primitive.
class YETI_PUBLIC Event {
 YETI_DISALLOW_COPYING(Event)

 public:
  /// \param @manual Requires manual reset after being signaled.
  Event(bool manual = false);

  ~Event();

 public:
  /// \brief Signals the event.
  void signal();

  /// \brief Unsignals the event.
  void unsignal();

  /// \brief Determines if the event was signaled.
  /// \warning Will unsignal the event if *not* a manually unsignaled event.
  bool signaled();

  /// \brief Waits for the eventt to be signaled.
  /// \note Will unsignal the event if *not* a manually unsignaled event.
  void wait();

 private:
  struct Storage;
  Storage *storage_;
};

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_EVENT_H_
