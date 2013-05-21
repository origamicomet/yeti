// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_LUA_SCRIPT_H_
#define _BUTANE_LUA_SCRIPT_H_

#include <butane/script.h>

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

namespace butane {
namespace Lua {
  class BUTANE_EXPORT Script final
    : public butane::Script
  {
    __foundation_trait(Script, non_copyable);

    private:
      friend class Stack;
      friend class Arguments;

    private:
      class Stack final
        : public butane::Script::Stack
      {
        __foundation_trait(Stack, non_copyable);

        public:
          Stack(
            Script& script );

          ~Stack();

        public:
          void push(
            void ) const override;

          void pop(
            void ) const override;

          void push(
            void* ptr ) const override;

          void pop(
            void*& ptr ) const override;

          void push(
            bool boolean ) const override;

          void pop(
            bool& boolean ) const override;

          void push(
            int integer ) const override;

          void pop(
            int& integer ) const override;

          void push(
            float number ) const override;

          void pop(
            float& number ) const override;

          void push(
            double number ) const override;

          void pop(
            double& number ) const override;

          void push(
            const char* string ) const override;

          void push(
            const String& string ) const override;

          void pop(
            String& string ) const override;

          void push(
            Function function ) const override;

          void pop(
            Function& function ) const override;
      };

      class Arguments final
        : public butane::Script::Arguments
      {
        __foundation_trait(Arguments, non_copyable);

        public:
          Arguments(
            Script& script );

          ~Arguments();

        public:
          operator size_t() const override;

        public:
          void to(
            size_t arg, void*& ptr ) const override;

          void to(
            size_t arg, bool& boolean ) const override;

          void to(
            size_t arg, int& integer ) const override;

          void to(
            size_t arg, float& number ) const override;

          void to(
            size_t arg, double& number ) const override;

          void to(
            size_t arg, const char*& string ) const override;

          void to(
            size_t arg, String& string ) const override;
      };

    public:
      Script();
      ~Script();

    private:
      static void* __alloc(
        Script* script,
        void* ptr,
        size_t old_size,
        size_t new_size );

      static int __forwarding_closure(
        lua_State* state );

      static int __error_handler(
        lua_State* state );

      static const char* __qualified_name(
        lua_State* state,
        const char* name );

    public:
      bool load(
        const char* name,
        const void* code,
        size_t code_len ) override;

      bool load(
        const String& name,
        const void* code,
        size_t code_len ) override;

      bool call(
        const char* name,
        size_t num_of_arguments,
        size_t& num_of_returns ) override;

      bool call(
        const String& name,
        size_t num_of_arguments,
        size_t& num_of_returns ) override;

      void expose(
        const char* name,
        Function function ) override;

      void expose(
        const String& name,
        Function function ) override;

    public:
      const butane::Script::Stack& stack() override;

    private:
      lua_State* _state;
      Stack _stack;
      size_t _memory_usage;
  };
} // Lua
} // butane

#endif // _BUTANE_LUA_SCRIPT_H_
