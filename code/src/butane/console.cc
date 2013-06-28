// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/console.h>

#include <butane/application.h>

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
      .connected((Network::Protocol::OnConnected)&Console::connected)
      .disconnected((Network::Protocol::OnDisconnected)&Console::disconnected)
      .local_to_remote("msg", (Network::Protocol::Constructor)&Console::__msg)
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
  ) : _conn(conn)
  {}

  Console::~Console()
  {
  }

  Console* Console::connected(
    Network::Protocol::Connection* conn )
  {
    assert(conn != nullptr);
    Console* console = make_new(Console, allocator())(conn);
    Application::consoles() += console;
    return console;
  }

  void Console::disconnected(
    Console* console )
  {
    assert(console != nullptr);
    Application::consoles().remove(Application::consoles().find(console));
    make_delete(Console, allocator(), console);
  }

  void Console::disconnect()
  {
    _conn->disconnect();
    _conn = nullptr;
  }

  void Console::log(
    const char* context,
    const char* message )
  {
    assert(context != nullptr);
    assert(message != nullptr);
    if (_conn)
      _conn->send("msg", (void*)this, context, message);
  }

  void Console::__msg(
    Console* console,
    Network::Packet& p,
    va_list ap )
  {
    p.write("context", va_arg(ap, const char*));
    p.write("msg", va_arg(ap, const char*));
  }

  bool Console::__on_cmd(
    Console* console,
    Network::Packet& p )
  {
    return true;
  }

  namespace Consoles {
    static thread_safe::Queue<Console::Message>& __messages_initializer() {
      static thread_safe::Queue<Console::Message> messages(
        Allocators::heap(), BUTANE_CONSOLE_MAXIMUM_NUM_OF_INFLIGHT_MESSAGES);
      return messages;
    }

    static const thread_safe::Static< thread_safe::Queue<Console::Message> >
      __ts_messages(&__messages_initializer);

    static thread_safe::Queue<Console::Message>& messages() {
      return __ts_messages();
    }

    static Thread::Return __background_server_thread(
      void* closure )
    {
      Network::Address addr;
      if (!Network::Address::from_string("127.0.0.1", addr))
        fail("Unable to start remote console server on '127.0.0.1:5000'; Network::Address::from_string failed!");
      addr.port() = (uint16_t)closure;
      if (!Console::protocol().host(&Consoles::update, addr))
        fail("Unable to start remote console server on '127.0.0.1:5000'; Network::Protocol::host failed!");
      return 0;
    }

    void initialize(
      const uint16_t port )
    {
      Thread(&__background_server_thread, (void*)port).run();
      add_logger(&Consoles::log);
    }

    void log(
      void* closure,
      const LogScope* log_scope,
      const char* format,
      va_list ap )
    {
      const size_t len = vsnprintf(nullptr, 0, format, ap) + 1;
      char* msg = (char*)alloca(len);
      vsnprintf(&msg[0], len, format, ap);
      messages().enqueue(Console::Message(log_scope ? log_scope->name() : "", msg));
    }

    void update()
    {
      while (!messages().empty()) {
        Console::Message msg;
        messages().dequeue(msg);
        for (auto iter = Application::consoles().begin(); iter != Application::consoles().end(); ++iter)
          (*iter)->log(msg.context.raw(), msg.msg.raw());
      }
    }
  } // Consoles
} // butane
