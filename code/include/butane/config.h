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
#if defined(BUTANE_DEBUG_BUILD)
  #define BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_WORKER_THREADS 1
#else
  #define BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_WORKER_THREADS 8
#endif

// The maximum number of in-flight tasks the task scheduler supports.
#define BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_INFLIGHT_TASKS 256

// The maximum number of in-flight messages the console supports.
#define BUTANE_CONSOLE_MAXIMUM_NUM_OF_INFLIGHT_MESSAGES 32

// The default port to use for a remote console.
#define BUTANE_CONSOLE_DEFAULT_PORT 5000

// The maximum number of seconds to wait when connecting to a remote console.
#define BUTANE_CONSOLE_TIMEOUT 10

// The minimum number of scene graphs to update per update_scene_graphs task.
#define BUTANE_MINIMUM_NUM_OF_SCENE_GRAPHS_TO_UPDATE_PER_TASK 256

// The maximum number of render contexts accepted by a dispatch task.
#define BUTANE_MAXIMUM_NUM_OF_DISPATCHABLE_RENDER_CONTEXTS 1

// The maximum number of bound textures/smaplers accepted by RenderConfig::Layers.
#define BT_LAYER_MAXIMUM_NUM_OF_SAMPLERS 8

// The maximum number of bound render targets accepted by RenderConfig::Layers.
#define BT_LAYER_MAXIMUM_NUM_OF_RENDER_TARGETS 8

// The maximum number of temporary math objects available to a script.
#define BT_SCRIPT_MAXIMUM_NUM_OF_TEMPORARY_MATH_OBJECTS 4096

#endif // _BUTANE_CONFIG_H_
