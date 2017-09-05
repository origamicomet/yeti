//===-- yeti/foundation/log.h ---------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_LOG_H_
#define _YETI_FOUNDATION_LOG_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace log {

enum Level {
  OFF     = 0x00000000,
  TRACE   = 0x00000001,
  DEBUG   = 0x00000002,
  INFO    = 0x00000003,
  WARNING = 0x00000004,
  ERROR   = 0x00000005,
  FATAL   = 0x00000006
};

struct Category {
  /// An opaque identifier assigned via `add`.
  typedef u32 Id;

  /// No category. None. Zilch.
  static const Category::Id NONE = 0xffffffff;

  /// Name of the category.
  ///
  /// \note Convention is to use a sigular noun or nouns in `snake_case`. For
  /// example, `resource_compiler` fits convention and is easily understood
  /// while `worlds (render)` does not and is extremely confusing in the
  /// context(s) it is used.
  char name[33];

  /// Parent category.
  ///
  /// \note Convention is to reference categories by recursively joining
  /// children with periods, e.g. `yeti.resource_manager`.
  Category::Id parent;

  /// Registers a category.
  static YETI_PUBLIC Category::Id add(
    const char *name,
    Category::Id parent = Category::NONE);
};

struct Message {
  struct {
    /// Refer to yeti::foundation::log::Level.
    unsigned level : 4;

    /// The user-specified "category" of message.
    /// Refer to yeti::foundation::log::Category.
    unsigned category : 8;

    /// Denotes weather `content` is structured or unstructured (plaintext).
    unsigned structured : 1;

    /// \internal Saved for later use.
    unsigned reserved : 19;

    /// System identifier for the thread this message originated from.
    u64 origin;
  } meta;

  u64 when;

  struct {
    const char *file;
    u32 line;
  } where;

  u32 content_len;
  u8 content[0];
};

struct Backend {
};

extern YETI_PUBLIC void print(log::Category::Id category,
                              log::Level level,
                              const char *message);

extern YETI_PUBLIC void printf(log::Category::Id category,
                               log::Level level,
                               const char *format, ...);

} // log
} // yeti

extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_GENERAL;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_CORE;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_MEMORY;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_SYSTEM;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_GRAPHICS;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_AUDIO;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_INPUT;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_SCRIPT;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_PROFILER;
extern YETI_PUBLIC const yeti::log::Category::Id YETI_LOG_APP;

#endif // _YETI_FOUNDATION_LOG_H_
