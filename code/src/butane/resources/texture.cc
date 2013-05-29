// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/texture.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("texture resources", Allocators::heap());
    return allocator;
  }

  const Resource::Type TextureResource::type(
    "texture", "dds",
    (Resource::Type::Load)&TextureResource::load,
    (Resource::Type::Unload)&TextureResource::unload,
    (Resource::Type::Compile)&TextureResource::compile);

  TextureResource::TextureResource(
    const Resource::Id id
  ) : butane::Resource(TextureResource::type, id)
    , _texture(nullptr)
  {
  }

  TextureResource::~TextureResource()
  {
    if (_texture)
      _texture->destroy();
  }

  TextureResource* TextureResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    // TextureResource* tr = make_new(TextureResource, allocator())(id);
    // const Header* header = (const Header*)stream.memory_resident_data();
    // copy((void*)&tr->_header, (const void*)header, sizeof(Header));
    // tr->_texture = Texture::create(header->type, header->pixel_format, header->width, header->height, header->depth, true);
    // tr->mark_for_streaming();
    // return tr;

    return nullptr;
  }

  void TextureResource::unload(
    TextureResource* texture )
  {
    assert(texture != nullptr);
    make_delete(TextureResource, allocator(), texture);
  }

  bool TextureResource::compile(
    const Resource::Compiler::Source& src,
    const Resource::Compiler::Stream& cs )
  {
    /* Determine if it is actually a DDS */ {
      char magic[4];
      if (!File::read_in(cs.source_data(), (void*)&magic[0], 4))
        return false;
      if (strncmp("DDS ", &magic[0], 4) != 0) {
        log("Not a DirectDraw Surface", src.path);
        return false; }
    }

    DDS_HEADER dds;
    if (!File::read_in(cs.source_data(), (void*)&dds, sizeof(DDS_HEADER)))
      return false;

    MemoryResidentData mrd; {
      mrd.width = dds.width;
      mrd.height = dds.height;
      mrd.flags = 0;

      if (!type_from_dds(dds, mrd.type)) {
        log("Invalid or unsupported texture type!");
        return false; }

      if (!pixel_format_from_dds(dds, mrd.pixel_format)) {
        log("Invalid or unsupported pixel format!");
        return false; }

      if (!num_of_faces_from_dds(dds, mrd.depth)) {
        log("Invalid or unsupported number of faces!");
        return false; }
    }

    if (dds.header_flags & DDS_HEADER_FLAGS_MIPMAP)
      mrd.flags |= HasMipmaps;

    if (!File::write_out(cs.memory_resident_data(), (const void*)&mrd, sizeof(MemoryResidentData)))
      return false;

    if (!File::copy(cs.source_data(), cs.streaming_data()))
      return false;

    return true;
  }

  // static thread_safe::Queue<TextureResource*>& __marked_for_streaming() {
  //   static thread_safe::Queue<TextureResource*> marked_for_streaming(
  //     Allocators::heap(), BUTANE_BACKGROUND_RESOURCE_UNLOADING_QUEUE_SIZE);
  //   return marked_for_streaming;
  // }

  // static Thread::Return __background_streaming_thread(
  //   Thread& thread,
  //   void* closure )
  // {
  //   while (true) {
  //     TextureResource* tr = nullptr;
  //     __marked_for_streaming().dequeue(tr);
  //     tr->stream();
  //     tr->dereference(); }

  //   __builtin_unreachable();
  //   return 0;
  // }

  // static void __start_background_streaming_thread()
  // {
  //   static bool has_started = false;
  //   if (has_started) return;
  //   static Thread background_streaming_thread(&__background_streaming_thread);
  //   background_streaming_thread.detach();
  //   has_started = true;
  // }

  // void TextureResource::stream()
  // {
  // }

  // void TextureResource::mark_for_streaming()
  // {
  //   // A reference is held to prevent the texture from being unloaded while the
  //   // background thread is streaming it in.
  //   reference();

  //   __start_background_streaming_thread();
  //   __marked_for_streaming().enqueue(this);
  // }
} // butane
