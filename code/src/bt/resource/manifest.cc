// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource/manifest.h>

namespace bt {
  static const Resource::Type _type(
    "manifest",
    "manifest",
    (Resource::Type::Load)&Manifest::load,
    (Resource::Type::Unload)&Manifest::unload,
    (Resource::Type::Compile)&Manifest::compile);

  Manifest::Manifest( Resource::Id id )
    : Resource(id, _type)
    , _root(nullptr)
  {
  }

  Manifest::~Manifest()
  {
    if (_root)
      make_delete(Value, Allocator::heap(), _root);
  }

  Manifest* Manifest::load(
    bt::Resource::Stream* stream )
  {
    fail("Manifest::load is not implemented, yet.");
    return nullptr;
  }

  void Manifest::unload(
    Manifest* resource )
  {
    fail("Manifest::unload is not implemented, yet.");
  }

  bt::Compile::Status Manifest::compile(
    const bt::Compile::Data& compile_data )
  {
    if (!File::copy(
          compile_data.input(),
          compile_data.memory_resident_data()))
    {
      return Compile::Failed;
    }

    return Compile::Successful;
  }

  String Manifest::to_string() const
  {
    return String(Allocator::scratch(),
      "#<Manifest:%016" PRIx64 "> "
      "[resource [id=[hash=%08X,revision=%u],type=%016" PRIx64 ",references=%u] : "
      "root=%016" PRIx64 "]",
      (uint64_t)this,
      id().hash(), id().revision(), (uint64_t)&type(), references(),
      (uint64_t)_root);
  }
} // bt