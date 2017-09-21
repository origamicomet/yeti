//===-- yeti/core/log.h ---------------------------------*- mode: C++11 -*-===//
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
/// \file
/// \brief Logging infrastructure.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_LOG_H_
#define _YETI_CORE_LOG_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

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
    /// Opaque identifier assigned when registered.
    typedef u32 Id;

    /// No category. None. Zilch.
    static const Category::Id NONE = 0;

    /// Parent category.
    ///
    /// \note Convention is to reference categories by recursively joining
    /// children with periods, e.g. `yeti.tools.resource_compiler`.
    ///
    Category::Id parent;

    /// Name of the category.
    ///
    /// \note Convention is to use a sigular noun or nouns in snake case. For
    /// example, `resource_compiler` fits convention and is easily understood
    /// while `worlds (render)` does not and is extremely confusing in the
    /// contexts it is used.
    ///
    char name[64];

    /// Registers a category.
    static YETI_PUBLIC Category::Id add(const char *name,
                                        Category::Id parent = Category::NONE);
  };

  extern YETI_PUBLIC const Category::Id GENERAL;
  extern YETI_PUBLIC const Category::Id APP;
  extern YETI_PUBLIC const Category::Id SCRIPT;
  extern YETI_PUBLIC const Category::Id SYSTEM;
  extern YETI_PUBLIC const Category::Id CORE;
  extern YETI_PUBLIC const Category::Id MEMORY;
  extern YETI_PUBLIC const Category::Id GRAPHICS;
  extern YETI_PUBLIC const Category::Id AUDIO;
  extern YETI_PUBLIC const Category::Id INPUT;
  extern YETI_PUBLIC const Category::Id NETWORK;
  extern YETI_PUBLIC const Category::Id PROFILER;
  extern YETI_PUBLIC const Category::Id DEBUGGER;
  extern YETI_PUBLIC const Category::Id TOOLS;

  struct Message {
    struct {
      /// The user-specified category of message.
      /// Refer to `yeti::core::log::Category`.
      unsigned category : 8;

      /// Level of message.
      /// Refer to `yeti::core::log::Level`.
      unsigned level : 4;

      /// Denotes weather `content` is structured or unstructured (plaintext).
      unsigned structured : 1;

      /// \internal Reserved for later use.
      unsigned reserved : 19;

      /// System identifier for the thread this message originated from.
      u64 origin;
    } meta;

    /// When this message was logged.
    /// Given in milliseconds since Unix epoch.
    u64 when;

  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
    /// Location in source tree where this message originated from.
    struct {
      const char *file;
      u32 line;
    } where;
  #endif

    u32 length;
    u8 content[0];
  };

  class YETI_PUBLIC Backend {
   YETI_DISALLOW_COPYING(Backend)

   protected:
    Backend(const char *name);
    virtual ~Backend();

   private:
    /// \internal Registers this backend.
    void add(const char *name);

    /// \internal Deregisters this backend.
    void remove();

   public:
    virtual void log(const Message &message) = 0;

   private:
    u32 id_;
  };
}

/// Logs an unstructured message of @level under @category.
extern YETI_PUBLIC void logf(log::Category::Id category,
                             log::Level level,
                             const char *format,
                             ...);

} // core
} // yeti

#endif // _YETI_CORE_LOG_H_
