// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_LUA_SCRIPT_H_
#define _BUTANE_LUA_SCRIPT_H_

#include <bt/script.h>

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

namespace bt {
namespace Lua {
  class Script
    : public bt::Script
  {
    FOUNDATION_NON_COPYABLE(Script);

    public:
      class Stack
        : public bt::Script::Stack
      {
        FOUNDATION_NON_COPYABLE(Stack);

        private:
          friend class Script;

        private:
          Stack( Script* script );
          ~Stack();

        public:
          void push( void );
          void push( void* ptr );
          void push( bool boolean );
          void push( int integer );
          void push( float number );
          void push( double number );
          void push( const char* string );
          void push( Function func );

          void pop( void );
          void pop( void*& ptr );
          void pop( bool& boolean );
          void pop( int& integer );
          void pop( float& number );
          void pop( double& number );
          void pop( String& string );
          void pop( Function& func );
      };

      class Arguments
        : public bt::Script::Arguments
      {
        FOUNDATION_NON_COPYABLE(Arguments);

        private:
          friend class Script;

        private:
          Arguments( Script* script );
          ~Arguments();

        public:
          // Returns the number of arguments.
          operator size_t();

        public:
          template <typename T>
          T to( size_t index );
      };

    private:
      friend class Stack;
      friend class Arguments;

    private:
      Script();
      ~Script();

    public:
      static Script* load(
        const void* code,
        size_t code_len,
        const char* name = nullptr );

    public:
      bool call(
        const char* name,
        size_t num_arguments,
        size_t& num_returns );

      void expose(
        const char* name,
        Script::Function func );

      void callstack(
        Callstack& callstack );

    private:
      static void* alloc(
        Script* script,
        void* ptr,
        size_t old_size,
        size_t new_size );

    public:
      Stack& stack()
      { return _stack; }

    public:
      String to_string();

    public: /* private */
      lua_State* _state;
      size_t _memory_usage;
      Stack _stack;
      Arguments _arguments;
  };
} // Lua
} // bt

#endif // _BUTANE_LUA_SCRIPT_H_