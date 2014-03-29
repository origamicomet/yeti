//=== bitbyte/butane/task_scheduler.h ========================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/butane/task_scheduler.h
/// @brief ...
///

#ifndef _BITBYTE_BUTANE_TASK_SCHEDULER_H_
#define _BITBYTE_BUTANE_TASK_SCHEDULER_H_

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>
#include <bitbyte/butane/task.h>

namespace bitbyte {
namespace butane {

/// ...
extern BITBYTE_BUTANE_EXPORT class TaskScheduler &task_scheduler();

/// @brief
///
class BITBYTE_BUTANE_EXPORT TaskScheduler {
  private: // constructors:
    TaskScheduler(size_t qsz);

  private: // copy-constructors:
    TaskScheduler(const TaskScheduler &task_scheduler);

  private: // assignment operators:
    TaskScheduler &operator=(const TaskScheduler &task_scheduler);

  private: // destructor:
    ~TaskScheduler();

  public: // management:
    /// ...
    static void initialize(size_t internal_queue_sz);

    /// ...
    void shutdown();

  public: // methods:
    /// ...
    void submit(Task &task);

    /// ...
    void do_work_while_waiting_for(foundation::Event &event);

  private: // methods:
    /// ...
    void schedule(Task *task);

    /// ...
    void consume();

  private: // members:
    foundation::lockfree::MpMcQueue<Task *> schedulable_;
    foundation::Thread **worker_threads_;
    size_t num_worker_threads_;
};

} // butane
} // bitbyte

#endif // _BITBYTE_BUTANE_TASK_SCHEDULER_H_
