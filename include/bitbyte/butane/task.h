//=== bitbyte/foundation/butane/task.h =======================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/foundation/butane/task.h
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
///
struct BITBYTE_BUTANE_EXPORT Task {
  public: // types:
    /// ...
    typedef foundation::tier3::Function<void, Task &> Kernel;

    /// ...
    typedef foundation::atomic::u32 RefCount;

  public: // static methods:
    /// ...
    static Task &describe();

  public: // methods:
    /// ...
    Task &parent(Task *parent);

    /// ...
    Task &dependency(Task *dependency);

    /// ...
    Task &kernel(Kernel &kernel);

  private: // members:
    Task *parent_;
    Task *dependency_;
    Task::Kernel kernel_;
    Task::RefCount refs_by_children_and_self_;
    Task::RefCount refs_by_dependencies_;
};

} // butane
} // bitbyte

#include <bitbyte/butane/task.inl>

#endif // _BITBYTE_BUTANE_TASK_H_
