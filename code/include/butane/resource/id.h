// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_ID_H_
#define _BUTANE_RESOURCE_ID_H_

class BUTANE_EXPORT Id final
{
  public:
    Id();

    Id(
      const Type& type,
      const char* path );

    Id(
      const Type& type,
      const String& path );

    Id(
      const Id& id );

    Id& operator= (
      const Id& id );

  public:
    FOUNDATION_INLINE bool operator== (
      const Resource::Id& id ) const
    { return (_hash == id._hash); }

    FOUNDATION_INLINE bool operator!= (
      const Resource::Id& id ) const
    { return (_hash != id._hash); }

    FOUNDATION_INLINE bool operator< (
      const Resource::Id& id ) const
    { return (_hash < id._hash); }

    FOUNDATION_INLINE bool operator> (
      const Resource::Id& id ) const
    { return (_hash > id._hash); }

  public:
    operator uint64_t() const;

  private:
    uint64_t _hash;
};

#endif // _BUTANE_RESOURCE_ID_H_
