// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCRIPT_H_
#define _BUTANE_SCRIPT_H_

#include <bt/foundation.h>
#include <bt/config.h>

namespace bt {
  class Script
  {
    FOUNDATION_NON_COPYABLE(Script);

    public:
      class Stack;
      class Arguments;

      // Represents a script function.
      // typedef size_t (*Callback)(
      //  Script::Arguments& args );

      // Represents a C/C++ function.
      typedef size_t (*Function)(
        Script* script,
        Script::Arguments& args );

      // Represents a C/C++ function to be called when a ->call fails.
      typedef void (*OnError)(
        Script* script,
        const char* error_msg,
        Callstack& callstack );

      class Stack {
        FOUNDATION_NON_COPYABLE(Stack);

        protected:
          Stack( Script* script );
          virtual ~Stack();

        public:
          virtual void push( void ) = 0;
          virtual void push( void* ptr ) = 0;
          virtual void push( bool boolean ) = 0;
          virtual void push( int integer ) = 0;
          virtual void push( float number ) = 0;
          virtual void push( double number ) = 0;
          virtual void push( const char* string ) = 0;
          virtual void push( Function func ) = 0;

          virtual void pop( void ) = 0;
          virtual void pop( void*& ptr ) = 0;
          virtual void pop( bool& boolean ) = 0;
          virtual void pop( int& integer ) = 0;
          virtual void pop( float& number ) = 0;
          virtual void pop( double& number ) = 0;
          virtual void pop( String& string ) = 0;
          virtual void pop( Function& func ) = 0;

        protected:
          Script* _script;
      };

      class Arguments {
        FOUNDATION_NON_COPYABLE(Arguments);

        protected:
          Arguments( Script* script );
          virtual ~Arguments();

        public:
          virtual operator size_t() = 0;

        protected:
          Script* _script;
      };

    protected:
      Script();
      virtual ~Script();

    public:
      static Script* load(
        const void* code,
        size_t code_len,
        const char* name = nullptr );

    public:
      virtual bool call(
        const char* name,
        size_t num_arguments,
        size_t& num_returns ) = 0;

      virtual void expose(
        const char* name,
        Function func ) = 0;

      virtual void callstack(
        Callstack& callstack ) = 0;

    public:
    virtual Stack& stack() = 0;

    public:
      virtual String to_string();

    public: /* private */
      OnError _on_error;
  };

namespace ScriptInterface {
  extern void expose( Script* script );
} // ScriptInterface
} // bt

#endif // _BUTANE_SCRIPT_H_