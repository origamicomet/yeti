// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/render_context.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("render contexts", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  RenderContext::RenderContext()
    : _commands(allocator())
    , _commands_offset(0)
    , _stream(allocator())
    , _stream_offset(0)
  {
  }

  RenderContext::~RenderContext()
  {
  }

  void RenderContext::command(
    const Command::Key key,
    const void* cmd,
    size_t cmd_len )
  {
    if (_commands_offset >= _commands.size())
      _commands.resize(max(_commands.size(), (size_t)1) * 2);

    Command& cmd_ = _commands[_commands_offset++];
    cmd_.key = key;
    cmd_.offset = _stream_offset;

    while ((_stream_offset + cmd_len) >= _stream.size())
      _stream.resize(max(_stream.size(), (size_t)1) * 2);

    copy((void*)&_stream[_stream_offset], cmd, cmd_len);
    _stream_offset += cmd_len;
  }
} // butane
