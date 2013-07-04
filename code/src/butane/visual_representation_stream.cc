// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/visual_representation_stream.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("visual representation streams", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  VisualRepresentationStream::VisualRepresentationStream()
    : _stream_offs(0)
    , _stream(allocator())
  {
  }

  VisualRepresentationStream::~VisualRepresentationStream()
  {
  }

  void VisualRepresentationStream::create(
    const VisualRepresentation::Type type,
    VisualRepresentation::Id* id )
  {
    assert(id != nullptr);
    assert(type != VisualRepresentation::INVALID);

    Requests::Create r;
    r.vrt = type;
    r.vrid = id;
    request((const void*)&r, sizeof(Requests::Create));
  }

  VisualRepresentation* VisualRepresentationStream::update(
    const VisualRepresentation::Id id )
  {
    if (id == VisualRepresentation::invalid)
      return nullptr;

    Requests::Update r;
    r.vrid = id;
    request((const void*)&r, sizeof(Requests::Update));

    const size_t visual_representation_len =
      VisualRepresentation::length(VisualRepresentation::type(id));

    while ((_stream_offs + visual_representation_len) < _stream.size())
      _stream.resize(max((size_t)1, _stream.size()) * 2);

    VisualRepresentation* visual_representation =
      (VisualRepresentation*)&_stream[_stream_offs];
    _stream_offs += visual_representation_len;

    return visual_representation;
  }

  void VisualRepresentationStream::destroy(
    const VisualRepresentation::Id id )
  {
    if (id == VisualRepresentation::invalid)
      return;

    Requests::Destroy r;
    r.vrid = id;
    request((const void*)&r, sizeof(Requests::Destroy));
  }

  void VisualRepresentationStream::request(
    const void* request,
    const size_t request_len )
  {
    assert(request != nullptr);
    assert(request_len > 0);

    while ((_stream_offs + request_len) < _stream.size())
      _stream.resize(max((size_t)1, _stream.size()) * 2);
    copy((void*)&_stream[_stream_offs], (const void*)request, request_len);
    _stream_offs += request_len;
  }

  void VisualRepresentationStream::for_each_request(
    void (*callback)(
      void* closure,
      const Request* request ),
    void* closure ) const
  {
    assert(callback != nullptr);

    size_t _offs = 0;
    while (_offs < _stream_offs) {
      const Request* request =
        (const Request*)&_stream[_offs];
      callback(closure, request);
      switch (request->type) {
        case Request::CREATE:
          _offs += sizeof(Requests::Create); break;
        case Request::UPDATE:
          _offs += sizeof(Requests::Update);
          _offs += VisualRepresentation::length(
            VisualRepresentation::type(((const Requests::Update*)request)->vrid));
          break;
        case Request::DESTROY:
          _offs += sizeof(Requests::Destroy); break;
        default:
          __builtin_unreachable();
      }
    }
  }
} // butane
