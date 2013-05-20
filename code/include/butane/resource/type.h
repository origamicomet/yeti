// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_TYPE_H_
#define _BUTANE_RESOURCE_TYPE_H_

class Type final {
  __foundation_trait(Type, non_copyable);

  public:
    typedef ImmutableHash<uint32_t, murmur_hash> Hash;

  public:
    typedef Resource* (*Load)(
      const Resource::Id id,
      const Resource::Stream& stream );
    
    typedef void (*Unload)(
      Resource* resource );

    typedef bool (*Compile)(
      void );

  public:
    Type(
      Hash name,
      const char* assoc_file_ext,
      Load load,
      Unload unload,
      Compile compile );

  private:
    void expose() const;

  public:
    FOUNDATION_INLINE const Hash& name() const
    { return _name; }

    FOUNDATION_INLINE const String& associated_file_extension() const
    { return _assoc_file_ext; }

    FOUNDATION_INLINE Resource* load(
      const Resource::Id id,
      const Resource::Stream& stream ) const
    { return _load(id, stream); }

    FOUNDATION_INLINE void unload(
      Resource* resource ) const
    { return _unload(resource); }

    FOUNDATION_INLINE bool compile(
      void ) const
    { return _compile(); }

  private:
    Hash _name;
    String _assoc_file_ext;
    Load _load;
    Unload _unload;
    Compile _compile;
};

#endif // _BUTANE_RESOURCE_TYPE_H_
