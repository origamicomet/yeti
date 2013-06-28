// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONSOLE_H_
#define _BUTANE_CONSOLE_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Console final {
    __foundation_trait(Console, non_copyable);

    public: /* private */
      static const Network::Protocol& protocol();

    public: /* private */
      class Message final {
        public:
          Message()
            : context(Allocators::scratch())
            , msg(Allocators::scratch())
          {}

          Message(
            const char* context,
            const char* msg
          ) : context(Allocators::scratch(), context)
            , msg(Allocators::scratch(), msg)
          {}

        public:
          String context;
          String msg;
      };

    private:
      Console(
        Network::Protocol::Connection* conn );

      ~Console();

    public: /* private */
      static Console* connected(
        Network::Protocol::Connection* conn );

      static void disconnected(
        Console* console );

    public:
      void log(
        const char* context,
        const char* message );

      void disconnect();

    public: /* private */
      static void __msg(
        Console* console,
        Network::Packet& p,
        va_list va );

      static bool __on_cmd(
        Console* console,
        Network::Packet& p );

    public:
      FOUNDATION_INLINE bool is_connected() const
      { return (_conn != nullptr); }

      FOUNDATION_INLINE bool is_disconnected() const
      { return (_conn == nullptr); }

    private:
      Network::Protocol::Connection* _conn;
  };

  namespace Consoles {
    extern BUTANE_EXPORT void initialize(
      const uint16_t port = BUTANE_CONSOLE_DEFAULT_PORT );

    extern BUTANE_EXPORT void log(
      void* closure,
      const LogScope* log_scope,
      const char* format,
      va_list ap );

    extern BUTANE_EXPORT void update();
  } // Consoles
} // butane

#endif // _BUTANE_CONSOLE_H_
