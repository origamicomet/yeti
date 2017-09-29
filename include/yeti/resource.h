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
  // Forward declared because callback signatures reference `Data`.
  struct Data;
}

namespace resource_compiler {
  // Forward declared to prevent cyclic dependencies.
  struct Environment;
  struct Input;
  struct Output;
}

namespace resource {
  /// An opaque identifier that uniquely identifies a resource.
  typedef u64 Id;

  /// Invalid resource identifier.
  static const Id INVALID = 0ull;

  /// Stage of a resource's lifecycle.
  enum State {
    /// Unknown.
    UNKNOWN = 0,

    /// Not loaded.
    UNLOADED = 1,

    /// Loaded.
    LOADED = 2,

    /// Failed to load.
    FAILED = 3
  };

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

    /// Invalid type identifier.
    static const Id INVALID = 0ul;

    /// Name of the resource type. Used as a unique identifier.
    ///
    /// \note Convention is to use a lowercase sigular nouns joined by
    /// underscores. `snake_case`. For example, `vertex_shader` fits convention
    /// and is easily understood while in the contexts it is used.
    ///
    const char *name;

    /// Source file extension(s) associated with the resource type.
    ///
    /// \note The end of array is denoted with `NULL`.
    ///
    const char **extensions;

    /// Arbitary number that determines the version of this type. Each resource
    /// of this type is tagged with this during compilation, allowing the
    /// engine and tools to quickly determine compatibility.
    ///
    /// \note If versions match, compatibility is assumed. When changing the
    /// format of compiled data, make sure to bump the version number and
    /// adjust the `compatible` callback accordingly.
    ///
    /// \note If version is `0`, incompatibility is always assumed. However,
    /// compatibility is ignored at runtime. Set version to `0` until you've
    /// stabilized your compiled data format.
    ///
    /// \remark If you want to properly maintain compatibility when dealing
    /// with a resource that contains data compiled by pluggable code, like
    /// `EntityResource`, `LevelResource`, and `RenderConfigResource` do, don't
    /// use this. Instead, maintain a separate file with contents that is
    /// deterministically derived from an internal versioning scheme, perhaps
    /// modeled after this, and make all resources dependent on it. Since the
    /// fingerprint of the file will only differ upon a version change,
    /// resources will only be recompiled when pluggable code changes, rather
    /// than everytime. This is also has the benefit of allowing you to version
    /// the container format separately.
    ///
    u32 version;

    /// Allocates space for a resource of this type.
    Resource *(*prepare)(resource::Id id);

    /// Loads a resource of this type from compiled data.
    void (*load)(Resource *resource, const resource::Data &data);

    /// Unloads a resource of this type.
    void (*unload)(Resource *resource);

    /// Brings a resource of this type online.
    void (*online)(Resource *resource);

    /// Brings a resource of this type offline.
    void (*offline)(Resource *resource);

    /// Tries to compile a resource of this type from source data.
    bool (*compile)(const resource_compiler::Environment *env,
                    const resource_compiler::Input *input,
                    const resource_compiler::Output *output);

    /// Determines if a resource compiled with @version is compatible with this
    /// version of code.
    bool (*compatible)(u32 version);

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

  struct File {
    /// An opaque identifier that uniquely identifies a source.
    typedef u32 Id;

    /// Unique identifier.
    Id id;

    typedef char Path[256];

    /// Relative path, including extensions.
    Path path;

    /// Last modified, in seconds from Unix epoch.
    u64 timestamp;

    /// Hash of contents.
    char fingerprint[40];
  };

  struct Override {
    resource::Id resource;
    resource::Id substitution;
  };

  struct Build {
    /// An opaque identifier that uniquely identifies a build.
    typedef u32 Id;

    struct Log {
      enum Level {
        /// Informational message.
        INFO = 1,

        /// Warning message.
        WARNING = 2,

        /// Error message.
        ERROR = 3
      };

      struct Message {
        Level level;
        const char *contents;
      };
    };

    /// Unique identifier.
    Id id;

    /// When the build was commenced, in seconds from Unix epoch.
    u64 started;

    /// When the build finished, in seconds from Unix epoch.
    u64 finished;

    /// Whether or not the build completed successfully.
    bool successful;
  };

  struct Dependency {
    enum Type {
      ONLINE  = 1,
      OFFLINE = 2
    };

    Type type;

    resource::Id resource;
    File::Id source;
  };

  /// Registers a resource type.
  ///
  /// \warning Registering types is thread-safe!
  ///
  /// \param @type Pointer to description of the type.
  ///
  /// \return Type identifier assigned to the type.
  ///
  extern YETI_PUBLIC Type::Id register_a_type(const Type *type);

  /// Finds a type by name.
  ///
  /// \param @name The name of the type to find.
  ///
  /// \return Named type or `NULL` if no type named @name exists
  ///
  extern YETI_PUBLIC const Type *type_from_name(const char *name);

  /// Determines the type of a resource from a path.
  ///
  /// \param @path Path to resource.
  ///
  /// \return Type of resource corresponding to @path or `NULL` if no type
  /// is responsible for the resource at @path.
  ///
  extern YETI_PUBLIC const Type *type_from_path(const char *path);

  /// Determines the type of a resource from an extension.
  ///
  /// \param @extension File extension.
  ///
  /// \return Type of resource corresponding to @extension or `NULL` if no type
  /// is responsible for files with the given extension.
  ///
  extern YETI_PUBLIC const Type *type_from_extension(const char *extension);

  /// Derives unique type identifier for a given type.
  ///
  /// \param @type Resource type.
  ///
  /// \return Unique identifier assigned to @type.
  ///
  extern YETI_PUBLIC Type::Id id_from_type(const Type *type);

  /// Derives type given a unique type identifier.
  ///
  /// \param @id Unique identifier assigned to a type.
  ///
  /// \return Type corresponding to @id or `NULL` if no type has been assigned
  /// the given identifier.
  ///
  extern YETI_PUBLIC const Type *type_from_id(Type::Id id);

  /// Derives the unique identifier for a resource given its path.
  ///
  /// \param @path Path to resource.
  ///
  /// \return Unique identifier corresponding to resource at @path.
  ///
  extern YETI_PUBLIC Id id_from_path(const char *path);

  /// Derives the unique identifier for a resource given its type and name.
  ///
  /// \param @path Path to resource.
  ///
  /// \return Unique identifier corresponding to resource at @path.
  ///
  extern YETI_PUBLIC Id id_from_name(Type::Id type, const char *name);

  /// Derives a resource's type and name given its unique identifier.
  extern YETI_PUBLIC void type_and_name_from_id(Id id, const Type **type, u32 *name);

  /// Calls @callback for every registered type.
  extern YETI_PUBLIC void for_each_type(void (*callback)(Type::Id id, const Type *type, void *context),
                                        void *context = NULL);
}

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

  /// \copydoc yeti::resource::File
  typedef resource::File File;

  /// \copydoc yeti::resource::Dependency
  typedef resource::Dependency Dependency;

  /// \copydoc yeti::resource::Override
  typedef resource::Override Override;

  /// \copydoc yeti::resource::Build
  typedef resource::Build Build;

  /// \copydoc yeti::resource::State
  typedef resource::State State;

  /// \copydoc yeti::resource::UNKNOWN
  static const State UNKNOWN = resource::UNKNOWN;

  /// \copydoc yeti::resource::UNLOADED
  static const State UNLOADED = resource::UNLOADED;

  /// \copydoc yeti::resource::LOADED
  static const State LOADED = resource::LOADED;

  /// \copydoc yeti::resource::FAILED
  static const State FAILED = resource::FAILED;

 public:
  /// A handle to a resource that automatically maintains reference count.
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

 protected:
  Resource(Resource::Id id);
  ~Resource();

 public:
  /// Returns the unique identifier for this resource.
  YETI_INLINE Id id() const {
    return id_;
  }

  /// \internal Returns the number of references to this resource.
  YETI_INLINE u32 refs() const {
    return atomic::load(&references_);
  }

 public:
  /// \brief Increase reference count of this resource.
  void ref() const;

  /// \brief Decreases reference count of this resource.
  ///
  /// \note If the caller holds the last reference, the resource will be queued
  /// for unloading.
  ///
  void deref() const;

 private:
  const Id id_;
  mutable u32 references_;
};

} // yeti

#endif // _YETI_RESOURCE_H_
