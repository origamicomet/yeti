// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/console.h>

// #include <butane/application.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("consoles", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static const Network::Protocol& __protocol_initializer() {
    static const Network::Protocol protocol = Network::Protocol("console", 1)
      .local_to_remote("log", (Network::Protocol::Constructor)&Console::__log)
      .remote_to_local("cmd", (Network::Protocol::Handler)&Console::__on_cmd);
    return protocol;
  }

  static const thread_safe::Static< const Network::Protocol >
    __ts_protocol(&__protocol_initializer);

  const Network::Protocol& Console::protocol()
  {
    return __ts_protocol();
  }

  Console::Console(
    Network::Protocol::Connection* conn
  ) : _refs(2)
    , _messages(allocator(), BUTANE_CONSOLE_MAXIMUM_NUM_OF_INFLIGHT_MESSAGES)
    , _conn(conn)
  {}

  Console::~Console()
  {
    if (_conn)
      _conn->disconnect();
  }

  Console* Console::connect(
    const Network::Address& remote,
    const size_t timeout )
  {
    Network::Protocol::Connection* conn =
      Console::protocol().connect(remote, timeout * 1000);
    if (!conn)
      return nullptr;
    Console* console = make_new(Console, allocator())(conn);
    Thread((Thread::Entry)&Console::__background_thread, (void*)console).run();
    return console;
  }

  void Console::disconnect()
  {
    if (__sync_fetch_and_sub(&_refs, 1) <= 1)
      make_delete(Console, allocator(), this);
  }

  void Console::log(
    const char* context,
    const char* message )
  {
    assert(context != nullptr);
    assert(message != nullptr);
    _messages.enqueue(Message(context, message));
  }

  void Console::__log(
    Console* console,
    Network::Packet& p,
    va_list ap )
  {
    p.write("context", va_arg(ap, const char*));
    p.write("msg", va_arg(ap, const char*));
  }

//   static void __script_error_handler(
//     Script& script,
//     Console* console,
//     /* const String& error
//     const Script::Callstack& callstack */ )
//   {
//     console->log(nullptr, error.raw());
//   }

  bool Console::__on_cmd(
    Console* console,
    Network::Packet& p )
  {
    // const char* cmd;
    // if (!p.read("cmd", cmd))
    //   return false;
    // Application::exec(cmd, (Script::ErrorHandler)&__script_error_handler, (void*)this);
    return true;
  }

  Thread::Return Console::__background_thread(
    Console* console )
  {
    assert(console != nullptr);

    while (!__sync_bool_compare_and_swap(&console->_refs, 1, 1)) {
      if (console->is_connected()) {
        while (!console->_messages.empty()) {
          Message msg;
          console->_messages.dequeue(msg);
          if (console->_conn)
            console->_conn->send("log", (void*)console, msg.context.raw(), msg.msg.raw());
        }

        console->_conn->update((void*)console);
      } else {
        console->_conn = Console::protocol().connect(console->_remote, 10000);
      }
    }

    console->disconnect();
    return 0;
  }
} // butane
