// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource/texture.h>

#include "dds.h"

namespace butane {
  static bool type_from_dds(
    const DDS_HEADER& dds,
    Texture::Type& type )
  {
    if (dds.surface_flags & DDS_SURFACE_FLAGS_TEXTURE) {
      if (dds.header_flags & DDS_HEADER_FLAGS_DEPTH) {
        type = ((dds.depth > 1) ? Texture::TEXTURE_2D_ARRAY : Texture::TEXTURE_2D);
        return true; }
      type = Texture::TEXTURE_2D;
      return true;
    } else if (dds.surface_flags & DDS_SURFACE_FLAGS_CUBEMAP) {
      if ((dds.cubemap_flags & DDS_CUBEMAP_ALLFACES) == DDS_CUBEMAP_ALLFACES) {
        type = Texture::CUBE_MAP; return true; }
      return false;
    }

    return false;
  }

  static bool pixel_format_from_dds(
    const DDS_HEADER& dds,
    PixelFormat& pixel_format )
  {
    typedef Pair<const DDS_PIXELFORMAT*, const PixelFormat> Pair;
    static const Pair fmts[] = {
      Pair(&DDSPF_DXT1, PixelFormat::DXT1),
      Pair(&DDSPF_DXT3, PixelFormat::DXT3),
      Pair(&DDSPF_DXT5, PixelFormat::DXT5),
      Pair(&DDSPF_A8R8G8B8, PixelFormat::R8G8B8A8)
    };

    for (size_t fmt = 0; fmt < (sizeof(fmts) / sizeof(fmts[0])); ++fmt) {
      if (compare((const void*)fmts[fmt].key, (const void*)&dds.pixel_format, sizeof(DDS_PIXELFORMAT))) {
        pixel_format = fmts[fmt].value; return true; } }

    return false;
  }

  static uint32_t num_of_faces_from_dds(
    const DDS_HEADER& dds,
    uint32_t& num_of_faces )
  {
    if (dds.surface_flags & DDS_SURFACE_FLAGS_TEXTURE) {
      num_of_faces = ((dds.header_flags & DDS_HEADER_FLAGS_DEPTH) ? dds.depth : 1);
      return true;
    } else if (dds.surface_flags & DDS_SURFACE_FLAGS_CUBEMAP) {
      num_of_faces = (((dds.cubemap_flags & DDS_CUBEMAP_ALLFACES) == DDS_CUBEMAP_ALLFACES) ? 6 : 0);
      return true; }

    return false;
  }
} // butane

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
    TextureResource* tr = make_new(TextureResource, allocator())(id);
    const Header* header = (const Header*)stream.memory_resident_data();
    copy((void*)&tr->_header, (const void*)header, sizeof(Header));
    tr->_texture = Texture::create(header->type, header->pixel_format, header->width, header->height, header->depth, true);
    tr->mark_for_streaming();
    return tr;
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

    Header header; {
      header.width = dds.width;
      header.height = dds.height;
      header.flags = 0;

      if (!type_from_dds(dds, header.type)) {
        log("Invalid or unsupported texture type!");
        return false; }

      if (!pixel_format_from_dds(dds, header.pixel_format)) {
        log("Invalid or unsupported pixel format!");
        return false; }

      if (!num_of_faces_from_dds(dds, header.depth)) {
        log("Invalid or unsupported number of faces!");
        return false; }
    }

    if (dds.header_flags & DDS_HEADER_FLAGS_MIPMAP)
      header.flags |= HasMipmaps;

    if (!File::write_out(cs.memory_resident_data(), (const void*)&header, sizeof(Header)))
      return false;

    if (!File::copy(cs.source_data(), cs.streaming_data()))
      return false;

    return true;
  }

  static thread_safe::Queue<TextureResource*>& __marked_for_streaming() {
    static thread_safe::Queue<TextureResource*> marked_for_streaming(
      Allocators::heap(), BUTANE_BACKGROUND_RESOURCE_UNLOADING_QUEUE_SIZE);
    return marked_for_streaming;
  }

  static Thread::Return __background_streaming_thread(
    Thread& thread,
    void* closure )
  {
    while (true) {
      TextureResource* tr = nullptr;
      __marked_for_streaming().dequeue(tr);
      tr->stream();
      tr->dereference(); }

    __builtin_unreachable();
    return 0;
  }

  static void __start_background_streaming_thread()
  {
    static bool has_started = false;
    if (has_started) return;
    static Thread background_streaming_thread(&__background_streaming_thread);
    background_streaming_thread.detach();
    has_started = true;
  }

  void TextureResource::stream()
  {
  }

  void TextureResource::mark_for_streaming()
  {
    // A reference is held to prevent the texture from being unloaded while the
    // background thread is streaming it in.
    reference();

    __start_background_streaming_thread();
    __marked_for_streaming().enqueue(this);
  }
} // butane
