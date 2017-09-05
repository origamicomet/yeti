//===-- yeti/foundation/log.cc --------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/log.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "yeti/foundation/atomics.h"

#include "yeti/foundation/log/console.h"

namespace yeti {

namespace log {
  namespace {
    static const u32 categories_sz_ = 256;
    static u32 num_categories_ = 10;
    static Category categories_[categories_sz_] = {
      {"yeti", Category::NONE},
      {"core", 0},
      {"memory", 0},
      {"system", 0},
      {"graphics", 0},
      {"audio", 0},
      {"input", 0},
      {"script", 0},
      {"profiler", 0},
      {"app", 0}
    };
  }
}

log::Category::Id log::Category::add(const char *name,
                                     log::Category::Id parent)
{
  yeti_assert_debug(name != NULL);
  yeti_assert_debug(strlen(name) <= sizeof(Category::name) - 1);

  const log::Category::Id id = foundation::atomic::increment(&num_categories_) - 1;
  yeti_assert_debug(id < categories_sz_);

  strncpy(&categories_[id].name[0], name, sizeof(Category::name));
  categories_[id].parent = parent;

  return id;
}

void log::print(log::Category::Id category,
                log::Level level,
                const char *message)
{
  yeti_assert_debug(message != NULL);
  yeti_assert_debug(category != Category::NONE);
  yeti_assert_debug(level != log::OFF);

  const size_t content_len = strlen(message) + 1;
  const size_t message_len = sizeof(Message) + content_len;

  // TODO(mtwilliams): Zero-copy.
  Message *msg = (Message *)alloca(message_len);

  // TODO(mtwilliams): Tag with `origin` thread.
  msg->meta.level = level;
  msg->meta.category = category;
  msg->meta.structured = 0;
  msg->meta.origin = 0;

  // TODO(mtwilliams): Tag with high-resolution time.
  msg->when = 0;

  msg->content_len = content_len;
  memcpy((void *)&msg->content[0], (const void *)message, content_len);

  // HACK(mtwilliams): Dump to console for the time being.
  console::log(msg);
}

void log::printf(log::Category::Id category,
                 log::Level level,
                 const char *format, ...)
{
  yeti_assert_debug(format != NULL);
  yeti_assert_debug(category != Category::NONE);
  yeti_assert_debug(level != log::OFF);

  va_list ap;
  va_start(ap, format);

  const int buf_sz = vsnprintf(NULL, 0, format, ap) + 1;

  va_end(ap);
  va_start(ap, format);

  char *buf = (char *)alloca(buf_sz);
  vsnprintf(buf, buf_sz, format, ap);

  print(category, level, (const char *)buf);

  va_end(ap);
}

} // yeti

// Refer to `categories_`.
const yeti::log::Category::Id YETI_LOG_GENERAL = 0;

const yeti::log::Category::Id YETI_LOG_SYSTEM = 1;
const yeti::log::Category::Id YETI_LOG_CORE = 2;
const yeti::log::Category::Id YETI_LOG_MEMORY = 3;
const yeti::log::Category::Id YETI_LOG_PROFILER = 4;
const yeti::log::Category::Id YETI_LOG_GRAPHICS = 5;
const yeti::log::Category::Id YETI_LOG_AUDIO = 6;
const yeti::log::Category::Id YETI_LOG_INPUT = 7;
const yeti::log::Category::Id YETI_LOG_SCRIPT = 8;
const yeti::log::Category::Id YETI_LOG_APP = 9;
