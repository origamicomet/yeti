//===-- yeti/resourceh ------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_RESOURCE_H_
#define _YETI_RESOURCE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

namespace resource_compiler {
  struct Input;
  struct Output;
}

// TODO(mtwilliams): Protect internal (to Yeti) interfaces.
 // Maybe a descendant `MutableResource`?

/// An instance of a resource.
///
/// \note There is only ever one instance of a resource except when being
/// reloaded.
///
class YETI_PUBLIC Resource {
 YETI_DISALLOW_COPYING(Resource);

 public:
  struct Data;

 public:
  /// An opaque identifier that uniquely identifies a resource.
  typedef u64 Id;

 public:
  /// Describes a type of resource.
  struct Type {
    /// An opaque identifier assigned to a type of resource based on its name.
    typedef u32 Id;

    /// Name of the resource type. Used as a unique identifier.
    /// \note Convention is to use a sigular noun or nouns in `snake_case`. For
    /// example, `vertex_shader` fits convention and is easily understood while
    /// `environments (sound)` does not and is extremely confusing in the
    /// context(s) it is used.
    const char *name;

    /// Source file extension(s) associated with the resource type.
    /// \note The end of array is marked with a sentinel value, `NULL`. For
    /// example, `{"vs", "vertex_shader", NULL}`.
    const char **extensions;

    Resource *(*prepare)(Resource::Id id);

    void (*load)(Resource *resource, const Resource::Data &data);
    void (*unload)(Resource *resource);

    void (*online)(Resource *resource);
    void (*offline)(Resource *resource);

    bool (*compile)(const resource_compiler::Input *input,
                    const resource_compiler::Output *output);
  };

 public:
  struct Data {
    /// Handle to memory-resident data.
    foundation::fs::File *memory_resident_data;

    /// Handle to read streaming data.
    foundation::fs::File *streaming_data;
  };

 public:
  /// \internal
  static Type::Id type_from_id(Resource::Id id);

  /// \internal
  static u32 hash_from_id(Resource::Id id);

  /// \internal
  static Resource::Id id_from_path(const char *path);

  /// \internal
  static Resource::Id id_from_type_and_name(Resource::Type::Id type, const char *name);

  /// \internal
  static Resource::Id id_from_type_and_hash(Resource::Type::Id type, u32 hash);

 public:
  /// A handle to a resource that automatically maintains reference counts.
  class Handle {
   public:
    explicit Handle(Resource *resource);
    Handle(const Resource::Handle &hndl);
    Handle &operator=(const Resource::Handle &hndl);
    ~Handle();

   public:
    operator Resource *() { return resource_; }

   private:
    mutable Resource *resource_;
  };

 public:
  ///
  enum State {
    UNLOADED = 0,
    LOADED   = 1,
    FAILED   = 2
  };

 protected:
  Resource(Resource::Id id);
  ~Resource();

 public:
  /// Returns the unique identifier for this resource.
  Id id() const { return id_; }

  /// \internal Returns the number of references to this resource.
  u32 refs() const { return foundation::atomic::load(&refs_); }

  /// \internal Adds a reference to this resource.
  void ref();

  /// \internal Removes a reference from this resource.
  /// \note If the caller holds the last reference, the resource will be queued
  /// for unloading.
  void deref();

  ///
  State state() const;

  ///
  void set_state(State state);

 private:
  const Id id_;
  u32 refs_;
  u32 state_;
};

} // yeti

#endif // _YETI_RESOURCE_H_
