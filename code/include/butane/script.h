// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCRIPT_H_
#define _BUTANE_SCRIPT_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Script abstract {
    __foundation_trait(Script, non_copyable);

    public:
      class Stack;
      class Arguments;
      class Type;

    public:
      // A C/C++ function that can be exposed to the script.
      typedef size_t (*Function)(
        Script& script,
        const Script::Arguments& arguments );

      // Called when a script encounters an error.
      typedef void (*ErrorHandler)(
        const Script& script,
        void* closure
        /* const String& error
        const Script::Callstack& callstack */ );

      // Called when a script is being destroyed.
      typedef void (*Destructor)(
        const Script& script );

    public:
      class Stack abstract {
        __foundation_trait(Stack, non_copyable);

        protected:
          Stack(
            Script& script
          ) : _script(script)
          {}

          virtual ~Stack()
          {}

        public:
          virtual void push(
            void ) const = 0;

          virtual void pop(
            void ) const = 0;

          virtual void push(
            void* ptr ) const = 0;

          virtual void pop(
            void*& ptr ) const = 0;

          virtual void push(
            bool boolean ) const = 0;

          virtual void pop(
            bool& boolean ) const = 0;

          virtual void push(
            int integer ) const = 0;

          virtual void pop(
            int& integer ) const = 0;

          virtual void push(
            float number ) const = 0;

          virtual void pop(
            float& number ) const = 0;

          virtual void push(
            double number ) const = 0;

          virtual void pop(
            double& number ) const = 0;

          virtual void push(
            const char* string ) const = 0;

          virtual void push(
            const String& string ) const = 0;

          virtual void pop(
            String& string ) const = 0;

          virtual void push(
            Function function ) const = 0;

          virtual void pop(
            Function& function ) const = 0;

          virtual void push(
            const char* type,
            void* ptr ) const = 0;

        protected:
          Script& _script;
      };

      class Arguments abstract {
        __foundation_trait(Arguments, non_copyable);

        protected:
          Arguments(
            Script& script
          ) : _script(script)
          {}

          virtual ~Arguments()
          {}

        public:
          virtual operator size_t() const = 0;

        public:
          virtual void to(
            size_t arg, void*& ptr ) const = 0;

          virtual void to(
            size_t arg, bool& boolean ) const = 0;

          virtual void to(
            size_t arg, int& integer ) const = 0;

          virtual void to(
            size_t arg, float& number ) const = 0;

          virtual void to(
            size_t arg, double& number ) const = 0;

          virtual void to(
            size_t arg, const char*& string ) const = 0;

          virtual void to(
            size_t arg, String& string ) const = 0;

          virtual void to(
            size_t arg, const char* type, void*& ptr ) const = 0;

        protected:
          Script& _script;
      };

      class Type abstract {
        __foundation_trait(Type, non_copyable);

        public:
          // A C/C++ function that is used to construct an instance a type.
          typedef void* (*Constructor)(
            Script& script,
            const Script::Arguments& arguments );

          // A C/C++ function that is used to destruct an instance of a type.
          typedef void (*Destructor)(
            Script& script,
            void* self );

          // A C/C++ funcation that operates on an instance.
          typedef size_t (*Method)(
            Script& script,
            void* self,
            const Script::Arguments& arguments );

          typedef Method Operator;
          typedef Method Getter;
          typedef Method Setter;

        protected:
          Type(
            Script& script
          ) : _script(script)
          {}

          virtual ~Type()
          {}

        public:
          virtual Type& getter(
            const char* member,
            Getter getter ) = 0;

          virtual Type& setter(
            const char* member,
            Setter setter ) = 0;

          virtual Type& accessors(
            const char* member,
            Getter getter,
            Setter setter ) = 0;

          virtual Type& operation(
            const char* operation,
            Operator method ) = 0;

          virtual Type& method(
            const char* name,
            Method method ) = 0;

          virtual Type& method(
            const char* name,
            Function function ) = 0;

          virtual void expose(
            const char* name ) = 0;

        protected:
          Script& _script;
      };

    protected:
      Script();
      virtual ~Script();

    public:
      static bool compile(
        const char* name,
        const char* code,
        size_t code_len,
        bool (*write)(
          void* closure,
          const void* bytes,
          size_t num_of_bytes ),
        void (*error)(
          void* closure,
          const char* error ),
        void* closure = nullptr );

      virtual bool load(
        const char* name,
        const void* code,
        size_t code_len ) = 0;

      virtual bool load(
        const String& name,
        const void* code,
        size_t code_len ) = 0;

      virtual bool call(
        const char* name,
        size_t num_of_arguments,
        size_t& num_of_returns ) = 0;

      virtual bool call(
        const String& name,
        size_t num_of_arguments,
        size_t& num_of_returns ) = 0;

      virtual Type& type(
        Type::Constructor ctor,
        Type::Destructor dtor ) = 0;

      virtual void expose(
        const char* name,
        Function function ) = 0;

      virtual void expose(
        const String& name,
        Function function ) = 0;

      virtual void set(
        const char* name ) = 0;

      virtual void set(
        const char* name,
        void* ptr ) = 0;

      virtual void get(
        const char* name,
        void*& ptr ) const = 0;

      virtual void set(
        const char* name,
        bool boolean ) = 0;

      virtual void get(
        const char* name,
        bool& boolean ) const = 0;

      virtual void set(
        const char* name,
        int integer ) = 0;

      virtual void get(
        const char* name,
        int& integer ) const = 0;

      virtual void set(
        const char* name,
        float number ) = 0;

      virtual void get(
        const char* name,
        float& number ) const = 0;

      virtual void set(
        const char* name,
        double number ) = 0;

      virtual void get(
        const char* name,
        double& number ) const = 0;

      virtual void set(
        const char* name,
        const char* string ) = 0;

      virtual void set(
        const char* name,
        const String& string ) = 0;

      virtual void get(
        const char* name,
        String& string ) const = 0;

    public:
      void error(
        const char* format, ... ) const;

    public:
      virtual const Stack& stack() = 0;

    public:
      FOUNDATION_INLINE void set_error_handler(
        ErrorHandler handler,
        void* closure = nullptr )
      {
        _error_handler = handler;
        _error_handler_closure = closure;
      }

      FOUNDATION_INLINE void on_destruction(
        Destructor destructor )
      {
        assert(destructor != nullptr);
        _destructors += destructor;
      }

    protected:
      ErrorHandler _error_handler;
      void* _error_handler_closure;
      Array<Destructor> _destructors;
  };
} // butane

#endif // _BUTANE_SCRIPT_H_
