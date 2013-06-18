// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONFIG_H_
#define _BUTANE_CONFIG_H_

#include <foundation/foundation.h>

// Uncomment this if you're linking to this as a library (and dynamically).
// #define BUTANE_DYNAMICALLY_LINKED 1

#if defined(BUTANE_DYNAMICALLY_LINKED)
  #if defined(BUTANE_COMPILING)
    #define BUTANE_EXPORT FOUNDATION_DYNAMIC_EXPORT
  #else
    #define BUTANE_EXPORT FOUNDATION_DYNAMIC_IMPORT
  #endif
#else
  #define BUTANE_EXPORT
#endif

// The size of the background resource unloading queue.
#define BUTANE_BACKGROUND_RESOURCE_UNLOADING_QUEUE_SIZE 256

// The initial size of the loaded resources hash-table.
#define BUTANE_LOADED_RESOURCES_HASH_TABLE_INITAL_SIZE 4096

// The initial size of the resource database hash-table.
#define BUTANE_RESOURCE_DATABASE_HASH_TABLE_INITAL_SIZE 4096

// The maximum number of worker threads the task scheduler can use.
#define BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_WORKER_THREADS 1

// The maximum number of in-flight tasks the task scheduler supports.
#define BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_INFLIGHT_TASKS 256

#endif // _BUTANE_CONFIG_H_
