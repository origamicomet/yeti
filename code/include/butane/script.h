// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCRIPT_H_
#define _BUTANE_SCRIPT_H_

#include <butane/butane.h>
#include <butane/lua.h>

namespace butane {
  class BUTANE_EXPORT Script final {
    __foundation_trait(Script, non_copyable);

    public:
      typedef void (*OnErrorHandler)(
        const Script& script,
        void* closure,
        const char* msg );

    public:
      Script();
      ~Script();

    private:
      static void* __alloc(
        Script* script,
        void* ptr,
        size_t old_size,
        size_t new_size );

      static void* __compiler_alloc(
        void* /* ud */,
        void* ptr,
        size_t old_size,
        size_t new_size );

      static int __error_handler(
        lua_State* state );

    public:
      static bool compile(
        const char* name,
        const char* code,
        const size_t code_len,
        bool (*write)(
          void* closure,
          const void* bytes,
          size_t num_of_bytes ),
        void (*error)(
          void* closure,
          const char* error ),
        void* closure = nullptr );

      bool load(
        const char* name,
        const void* code,
        const size_t code_len );

      int call(
        const char* name,
        const int num_of_arguments );

    public:
      void set_on_error_handler(
        OnErrorHandler handler,
        void* closure = nullptr );

    private:
      struct {
        void* closure;
        OnErrorHandler handler;
      } _on_error;

    private:
      lua_State* L;
      size_t _memory_usage;
  };
} // butane

#endif // _BUTANE_SCRIPT_H_
