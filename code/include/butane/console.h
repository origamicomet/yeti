// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONSOLE_H_
#define _BUTANE_CONSOLE_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Console final {
    __foundation_trait(Console, non_copyable);

    public:
      static const Network::Protocol& protocol();

    private:
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

    public:
      static Console* connect(
        const Network::Address& remote,
        const size_t timeout = BUTANE_CONSOLE_TIMEOUT );

      void disconnect();

    public:
      void log(
        const char* context,
        const char* message );

    public: /* private */
      static void __msg(
        Console* console,
        Network::Packet& p,
        va_list va );

      static bool __on_cmd(
        Console* console,
        Network::Packet& p );

    private:
      static Thread::Return __background_thread(
        Console* console );

    public:
      FOUNDATION_INLINE bool is_connected() const
      { return (_conn != nullptr); }

      FOUNDATION_INLINE bool is_disconnected() const
      { return (_conn == nullptr); }

    private:
      int32_t _refs;
      thread_safe::Queue<Message> _messages;
      Network::Address _remote;
      Network::Protocol::Connection* _conn;
  };
} // butane

#endif // _BUTANE_CONSOLE_H_
