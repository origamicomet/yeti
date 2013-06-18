// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASK_SCHEDULER_H_
#define _BUTANE_TASK_SCHEDULER_H_

class BUTANE_EXPORT Scheduler final {
  __foundation_trait(Scheduler, non_constructable);
  __foundation_trait(Scheduler, non_copyable);

  private:
    friend class Task;

  public:
    static void initialize();

    static void enqueue(
      Task* task );

  public:
    static size_t num_of_worker_treads();

  private:
    static Thread::Return worker_thread(
      void* closure );
};

#endif // _BUTANE_TASK_SCHEDULER_H_
