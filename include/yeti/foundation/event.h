//===-- yeti/foundation/event.h ---------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_EVENT_H_
#define _YETI_FOUNDATION_EVENT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// A waitable event.
class YETI_PUBLIC Event {
 YETI_DISALLOW_CONSTRUCTION(Event);
 YETI_DISALLOW_COPYING(Event);

 public:
  /// \brief Creates an event.
  /// \param @all Wake all waiting threads when signalled.
  static Event *create(bool all = false);

  /// \brief Destroys this event.
  void destroy();

 public:
  /// \brief Signals this event.
  void signal();

  /// \brief Unsignals this event.
  void unsignal();

  /// \brief Determines if this event was signalled.
  /// \warning Will unsignal this event if *not* created with |all|.
  bool signalled();

  /// \brief Waits on this event.
  void wait();

 private:
  struct Implementation;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_EVENT_H_
