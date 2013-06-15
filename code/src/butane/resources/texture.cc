// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/texture.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("texture resources", Allocators::heap());
    return allocator;
  }

  static Allocator& scratch() {
    static ProxyAllocator allocator("texture resources (scratch)", Allocators::heap());
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
    , _sampler(nullptr)
  {
  }

  TextureResource::~TextureResource()
  {
    if (_texture)
      _texture->destroy();
    if (_sampler)
      _sampler->destroy();
  }

  TextureResource* TextureResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("TextureResource::load");

    const MemoryResidentData& mrd =
      *((const MemoryResidentData*)stream.memory_resident_data());

    TextureResource* texture =
      make_new(TextureResource, allocator())(id);

    const size_t storage_requirements =
      mrd.pixel_format.storage_requirements(mrd.width, mrd.height, mrd.depth);

    void* buffer = scratch().alloc(storage_requirements);
    if (!File::read_in(stream.streaming_data(), buffer, storage_requirements))
      fail("Malformed streaming data for %016 (texture)" PRIx64);

    // http://stackoverflow.com/questions/6347950/programmatically-creating-directx-11-textures-pros-and-cons-of-the-three-differ
    texture->_texture = Texture::create(
      mrd.type, mrd.pixel_format, mrd.width, mrd.height, mrd.depth, mrd.flags, buffer);

    Sampler::Desc sd;
    sd.filter = (mrd.flags & Texture::HAS_MIP_MAPS) ?
      Sampler::Filter::ANISOTROPIC : Sampler::Filter::MIN_MAG_MIP_POINT;
    sd.uvw[0] = sd.uvw[1] = sd.uvw[2] = Sampler::TextureAddressingMode::WRAP;
    texture->_sampler = Sampler::create(sd);

    scratch().free(buffer);
    return texture;
  }

  void TextureResource::unload(
    TextureResource* texture )
  {
    const LogScope _("TextureResource::unload");

    assert(texture != nullptr);
    make_delete(TextureResource, allocator(), texture);
  }

  bool TextureResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("TextureResource::compile");

    /* Determine if it is actually a DDS */ {
      char magic[4];
      if (!File::read_in(input.data, (void*)&magic[0], 4))
        return false;
      if (strncmp("DDS ", &magic[0], 4) != 0) {
        log("Not a DirectDraw Surface", input.path);
        return false; }
    }

    DDS_HEADER dds;
    if (!File::read_in(input.data, (void*)&dds, sizeof(DDS_HEADER)))
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
      mrd.flags |= Texture::HAS_MIP_MAPS;

    if (!File::write_out(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData)))
      return false;

    if (!File::copy(input.data, output.streaming_data))
      return false;

    return true;
  }
} // butane
