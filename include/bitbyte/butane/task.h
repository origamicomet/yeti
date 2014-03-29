//=== bitbyte/butane/task.h ==================================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/butane/task.h
/// @brief ...
///

#ifndef _BITBYTE_BUTANE_TASK_H_
#define _BITBYTE_BUTANE_TASK_H_

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

namespace bitbyte {
namespace butane {

/// @brief
///
struct BITBYTE_BUTANE_EXPORT Task {
  friend class TaskScheduler;

  public: // types:
    /// ...
    typedef foundation::tier3::Function<void> Kernel;

    /// ...
    typedef foundation::atomic::s32 Permission;

    /// ...
    struct Permit {
      Permit *next;
      Task *task;
    };

  public: // static methods:
    /// ...
    static Task &describe();

  public: // methods:
    /// ...
    Task &parent(Task &parent);

    /// ...
    Task &dependency(Task &dependency);

    /// ...
    Task &kernel(Kernel &kernel);

  private: // members:
    Task::Permission permissions_;
    Task::Permit *permits_;
    Task::Kernel kernel_;
};

} // butane
} // bitbyte

#include <bitbyte/butane/task.inl>

#endif // _BITBYTE_BUTANE_TASK_H_
