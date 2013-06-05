// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONFIG_RESOURCE_H_
#define _BUTANE_CONFIG_RESOURCE_H_

#include <butane/resource.h>

namespace butane {
  class BUTANE_EXPORT ConfigResource final
    : public butane::Resource
  {
    __foundation_trait(ConfigResource, non_copyable);

    public:
      static const Resource::Type type;

    private:
      ConfigResource(
        const Resource::Id id );

      ~ConfigResource();

    private:
      static ConfigResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        ConfigResource* config );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      template <typename T>
      bool find(
        const char* name,
        T& value );

    private:
      const sjson::Object* _root;
  };
} // butane

#endif // _BUTANE_CONFIG_RESOURCE_H_
