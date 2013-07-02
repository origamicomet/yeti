// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASK_H_
#define _BUTANE_TASK_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Task final {
    __foundation_trait(Task, non_copyable);

    public:
      typedef Thread::Affinity Affinity;

      typedef void (*Kernel)(
        Task* task,
        uintptr_t data );

    public:
      class Scheduler;
      friend class Scheduler;

      #include <butane/tasks/scheduler.h>

    private:
      Task(
        const char* name,
        Affinity affinity,
        Task* parent,
        Task* depends_on,
        int32_t num_of_open_work_items,
        Kernel kernel,
        uintptr_t data );

      ~Task();

    public:
      static Task* prepare(
        const Affinity affinity,
        Kernel kernel,
        uintptr_t data,
        Task* depends_on = nullptr );

      Task* child(
        const Affinity affinity,
        Kernel kernel,
        uintptr_t data,
        Task* depends_on = nullptr );

      void kick();

      void kick_and_wait();

    private:
      const char* _name;
      Affinity _affinity;
      Task* _parent;
      Task* _depends_on;
      int32_t _num_of_open_work_items;
      int32_t _num_of_open_dependencies;
      Kernel _kernel;
      uintptr_t _data;
  };
} // butane

#endif // _BUTANE_TASK_H_
