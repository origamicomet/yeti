//===-- yeti/core/log.cc --------------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/log.h"

// For santiy checks.
#include "yeti/core/debug/assert.h"

// For thread safety.
#include "yeti/core/atomics.h"
#include "yeti/core/platform/lock.h"

// For thread identification.
#include "yeti/core/platform/thread.h"

namespace yeti {
namespace core {

namespace log {

static u32 num_of_categories_ = 13;
static Category categories_[255] = {
  {Category::NONE, "yeti"},
  {0,              "app"},
  {0,              "script"},
  {0,              "system"},
  {0,              "core"},
  {0,              "memory"},
  {0,              "graphics"},
  {0,              "audio"},
  {0,              "input"},
  {0,              "network"},
  {0,              "profiler"},
  {0,              "debug"},
  {0,              "tools"},
};

const Category::Id GENERAL  = 1;
const Category::Id APP      = 2;
const Category::Id SCRIPT   = 3;
const Category::Id SYSTEM   = 4;
const Category::Id CORE     = 5;
const Category::Id MEMORY   = 6;
const Category::Id GRAPHICS = 7;
const Category::Id AUDIO    = 8;
const Category::Id INPUT    = 9;
const Category::Id NETWORK  = 10;
const Category::Id PROFILER = 11;
const Category::Id DEBUGGER = 12;
const Category::Id TOOLS    = 13;

Category::Id Category::add(const char *name,
                           Category::Id parent) {
  yeti_assert_debug(name != NULL);
  yeti_assert_with_reason_debug(strlen(name) < sizeof(Category::name),
                                "Category names cannot exceed %u characters in length.",
                                sizeof(Category::name) - 1);
  yeti_assert_with_reason_debug(atomic::load(&num_of_categories_) <= 255,
                                "Cannot register anymore categories.");

  const Category::Id id = atomic::increment(&num_of_categories_);

  categories_[id - 1].parent = parent;
  strncpy(&categories_[id - 1].name[0], name, sizeof(Category::name));

  return id;
}

static Lock lock_;
static u32 num_of_backends_ = 0;
static Backend *backends_[255] = { NULL, };

Backend::Backend(const char *name) {
  this->add(name);
}

Backend::~Backend() {
  this->remove();
}

void Backend::add(const char *name) {
  yeti_assert_debug(name != NULL);
  yeti_assert_with_reason_debug(atomic::load(&num_of_backends_) <= 255,
                                "Cannot register anymore backends.");

  YETI_SCOPED_LOCK(lock_);

  id_ = atomic::increment(&num_of_backends_);
  backends_[id_ - 1] = this;
}

void Backend::remove() {
  YETI_SCOPED_LOCK(lock_);

  const u32 victim = atomic::decrement(&num_of_backends_);

  backends_[victim]->id_ = id_;
  backends_[id_ - 1] = backends_[victim];
}

// Delivers a message to all backends.
static void deliver(const Message &message) {
  YETI_SCOPED_LOCK(lock_);

  for (u32 backend = 0; backend < num_of_backends_; ++backend)
    backends_[backend]->log(message);
}

}

void logf(log::Category::Id category, log::Level level, const char *format, ...) {
  yeti_assert_debug(format != NULL);
  yeti_assert_debug(category != log::Category::NONE);
  yeti_assert_debug(level > log::OFF);

  va_list ap;

  va_start(ap, format);
  const int length = vsnprintf(NULL, 0, format, ap) + 1;
  va_end(ap);

  log::Message &message = *(log::Message *)alloca(sizeof(log::Message) + length);

  message.meta.category = category;
  message.meta.level = level;
  message.meta.structured = 0;
  message.meta.origin = Thread::id();

  // TODO(mtwilliams): Tag with high-resolution time.
  message.when = 0;

  // TODO(mtwilliams): Tag with originating location.
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
  message.where.file = NULL;
  message.where.line = 0;
#endif

  message.length = length;

  va_start(ap, format);
  vsnprintf((char *)&message.content[0], length, format, ap);
  va_end(ap);

  // PERF(mtwilliams): Collect messages to thread-local buffer and periodically
  // flush to a background delivery thread.
  log::deliver(message);
}

} // core
} // yeti
