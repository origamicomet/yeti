// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Type::Type(
    const char* name,
    const char* assoc_file_ext,
    Load load,
    Unload unload,
    Compile compile
  ) : _name(Allocators::heap(), name)
    , _assoc_file_ext(Allocators::heap(), assoc_file_ext)
    , _load(load), _unload(unload), _compile(compile)
  {
    expose();
  }

  static Array<const Resource::Type*>& types() {
    static Array<const Resource::Type*> types(Allocators::heap());
    return types;
  }

  void Resource::Type::expose() const
  {
    types() += (const Resource::Type*)this;
  }

  const Resource::Type* Resource::Type::determine(
    const Resource::Id id )
  {
    for (auto iter = types().begin(); iter != types().end(); ++iter) {
      if (Hash((*iter)->_name) == id.type())
        return *iter; }

    return nullptr;
  }

  const Resource::Type* Resource::Type::determine(
    const char* path )
  {
    if (!path)
      return nullptr;

    const String ext = Path::extension(String(Allocators::scratch(), path));
    const Hash hash(ext);

    for (auto iter = types().begin(); iter != types().end(); ++iter) {
      if (Hash((*iter)->_assoc_file_ext) == hash)
        return *iter; }

    return nullptr;
  }
} // butane
