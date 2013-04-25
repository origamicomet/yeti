// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource.h>

namespace bt {
  Resource::Type::Type(
    bt::Hash associated_extension,
    bt::Hash associated_source_extension,
    Load load,
    Unload unload,
    Compile compile
  ) : _assoc_extension(associated_extension)
    , _assoc_src_extensions(Allocator::heap(), 1)
    , _load(load), _unload(unload), _compile(compile)
  {
    _assoc_src_extensions.push_back(associated_source_extension);
    expose();
  }

  Resource::Type::Type(
    bt::Hash associated_extension,
    const Array<bt::Hash>& associated_source_extensions,
    Load load,
    Unload unload,
    Compile compile
  ) : _assoc_extension(associated_extension)
    , _assoc_src_extensions(associated_source_extensions)
    , _load(load), _unload(unload), _compile(compile)
  {
    expose();
  }

  bool Resource::Type::operator== ( const char* extension )
  {
    const Hash extension_h = Hash(extension);

    if (_assoc_extension == extension_h)
      return true;

    for (auto iter = _assoc_src_extensions.begin();
         iter != _assoc_src_extensions.end(); ++iter)
    {
      if (iter.to_ref() == extension_h)
        return true;
    }

    return false;
  }

  static Array<const Resource::Type*>& types() {
    static Array<const Resource::Type*> types(Allocator::heap());
    return types;
  }

  void Resource::Type::expose() const
  {
    // assert(search(type, types().to_ptr(), types().size()) == nullptr);
    types().push_back(this);
  }

  bt::Hash Resource::Type::associated_extension() const
  { return _assoc_extension; }

  const Array<bt::Hash>& Resource::Type::associated_source_extensions() const
  { return _assoc_src_extensions; }

  bt::Resource* Resource::Type::load(
    bt::Resource::Stream* stream ) const
  {
    assert(_load != nullptr);
    return _load(stream);
  }

  void Resource::Type::unload(
    bt::Resource* resource ) const
  {
    assert(_unload != nullptr);
    return _unload(resource);
  }

  bt::Compile::Status Resource::Type::compile(
    const bt::Compile::Data& compile_data ) const
  {
    assert(_compile != nullptr);
    return _compile(compile_data);
  }

  // ============================================================

  Resource::Resource( Id id, const Resource::Type& type )
    : _resource_id(id)
    , _resource_type(type)
    , _resource_refs(1)
  {
  }

  Resource::~Resource()
  {
  }

  const Resource::Type* Resource::determine_type_from_extension(
    const char* extension )
  {
    assert(extension != nullptr);

    const Hash extension_h = Hash(extension);
    for (auto iter = types().begin(); iter != types().end(); ++iter) {
      auto type = iter.to_ref();

      if (type->_assoc_extension == extension_h)
        return type;

      for (auto iter_ = type->_assoc_src_extensions.begin();
           iter_ != type->_assoc_src_extensions.end(); ++iter)
      {
        if (iter_.to_ref() == extension_h)
          return type;
      }
    }

    return nullptr;
  }

  String Resource::to_string()
  {
    return String(Allocator::scratch(),
      "#<Resource:%016" PRIx64 "> "
      "[id=[hash=%08X,revision=%u],type=%016" PRIx64 ",references=%u]",
      (uint64_t)this,
      id().hash(), id().revision(), (uint64_t)&type(), references());
  }
} // bt