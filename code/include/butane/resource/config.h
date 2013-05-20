// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONFIG_RESOURCE_H_
#define _BUTANE_CONFIG_RESOURCE_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/resource.h>

namespace butane {
  class ConfigResource final
    : public butane::Resource
  {
    public:
      static const Resource::Type& type();

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
        void );

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
