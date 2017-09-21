//===-- yeti/resource.h ---------------------------------*- mode: C++11 -*-===//
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

#include "yeti/core.h"

namespace yeti {

class Resource;

namespace resource {
  struct Type;
  struct Data;
}

namespace resource_compiler {
  struct Environment;
  struct Input;
  struct Output;
}

namespace resource {
  /// An opaque identifier that uniquely identifies a resource.
  typedef u64 Id;

  namespace LifecyclePreferences {
    /// Preference for how lifecycle callbacks should be called relative to
    /// other resources of the same type.
    ///
    /// \warning This is a preference, not a guarantee. While care is taken to
    /// adhere to your preference it will be ignored if doing so is too costly.
    ///
    enum _ {
      /// Don't care how resources are brought online.
      INDIFFERENT = 0,

      /// Prefer that resources are immediately brought online after loading.
      IMMEDIATE = 1,

      /// Prefer that resources of the same type are all loaded first, prior to
      /// being brought online.
      BATCH = 2,

      /// Prefer that resources are brought online just prior to first use.
      ///
      /// Use this if bringing a resource online after loading it causes stalls
      /// unless sufficient time has passed. This is seldom the case and should
      /// be used sparingly.
      JUST_IN_TIME = 3
    };
  }

  /// \copydoc yeti::resource::LifecyclePreferences::_
  typedef LifecyclePreferences::_ LifecyclePreference;

  /// Describes a type of resource.
  struct Type {
    /// An opaque identifier assigned to a type of resource based on its name.
    typedef u32 Id;

    /// Name of the resource type. Used as a unique identifier.
    ///
    /// \note Convention is to use a sigular noun or nouns in `snake_case`. For
    /// example, `vertex_shader` fits convention and is easily understood while
    /// `environments (sound)` does not and is extremely confusing in the
    /// contexts it is used.
    const char *name;

    /// Source file extension(s) associated with the resource type.
    ///
    /// \note The end of array is marked with a sentinel value, `NULL`. For
    /// example, `{"vs", "vertex_shader", NULL}`.
    const char **extensions;

    Resource *(*prepare)(resource::Id id);

    void (*load)(Resource *resource, const resource::Data &data);
    void (*unload)(Resource *resource);

    void (*online)(Resource *resource);
    void (*offline)(Resource *resource);

    bool (*compile)(const resource_compiler::Environment *env,
                    const resource_compiler::Input *input,
                    const resource_compiler::Output *output);

    /// \copydoc LifecyclePreference
    resource::LifecyclePreference lifecycle_preference;
  };

  /// Data available at runtime.
  struct Data {
    /// Handle to memory-resident data.
    core::File *memory_resident_data;

    /// Handle to read streaming data.
    core::File *streaming_data;
  };
}

// TODO(mtwilliams): Protect internal (to Yeti) interfaces.
 // Maybe a descendant `MutableResource`?

/// An instance of a resource.
///
/// \note There is only ever one instance of a resource except when being
/// reloaded.
///
class YETI_PUBLIC Resource {
 YETI_DISALLOW_COPYING(Resource)

 public:
  /// \copydoc yeti::resource::Id
  typedef resource::Id Id;

  /// \copydoc yeti::resource::Type
  typedef resource::Type Type;

  /// \copydoc yeti::resource::Data
  typedef resource::Data Data;

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
  u32 refs() const { return atomic::load(&refs_); }

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
