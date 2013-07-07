// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_HANDLE_H_
#define _BUTANE_RESOURCE_HANDLE_H_

template <typename T>
class BUTANE_EXPORT Handle final
{
  public:
    Handle()
      : _resource(nullptr)
    {}

    Handle(
      const Resource::Id id
    ) : _id(id)
      , _resource((T*)Resource::load(T::type(), _id))
    {}

    Handle(
      const char* path
    ) : _id(Resource::Id(T::type(), path))
      , _resource((T*)Resource::load(T::type(), _id))
    {}

    Handle(
      const String& path
    ) : _id(Resource::Id(T::type(), path.raw()))
      , _resource((T*)Resource::load(T::type(), _id))
    {}

    Handle(
      const Handle<T>& hndl
    ) : _id(hndl._id)
      , _resource((T*)Resource::load(T::type(), _id))
    {}

    Handle<T>& operator= (
      const Handle<T>& hndl )
    {
      if (&hndl == this)
        return *this;
      if (_resource)
        _resource->dereference();
      _id = hndl._id;
      _resource = (T*)Resource::load(T::type(), _id);
      return *this;
    }

    ~Handle()
    {
      if (_resource)
        _resource->dereference();
    }

  public:
    FOUNDATION_INLINE bool operator== (
      const Resource::Handle<T>& hndl ) const
    { return (_id == hndl._id); }

    FOUNDATION_INLINE bool operator!= (
      const Resource::Handle<T>& hndl ) const
    { return (_id != hndl._id); }

    FOUNDATION_INLINE bool operator< (
      const Resource::Handle<T>& hndl ) const
    { return (_id < hndl._id); }

    FOUNDATION_INLINE bool operator> (
      const Resource::Handle<T>& hndl ) const
    { return (_id > hndl._id); }

  public:
    FOUNDATION_INLINE operator T*() const
    { return _resource; }

    FOUNDATION_INLINE T* operator* () const
    { return _resource; }

    FOUNDATION_INLINE T* operator ->() const
    { return _resource; }

  public:
    FOUNDATION_INLINE bool valid() const
    { return (_resource != nullptr); }

  private:
    Id _id;
    T* _resource;
};

#endif // _BUTANE_RESOURCE_HANDLE_H_
