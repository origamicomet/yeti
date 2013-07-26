// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCRIPT_RESOURCE_H_
#define _BUTANE_SCRIPT_RESOURCE_H_

#include <butane/resource.h>

namespace butane {
  class BUTANE_EXPORT ScriptResource final
    : public butane::Resource
  {
    __foundation_trait(ScriptResource, non_copyable);

    public:
      static const Resource::Type& type();

    private:
      ScriptResource(
        const Resource::Id id );

      ~ScriptResource();

    public: /* private */
      static ScriptResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        ScriptResource* script );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      FOUNDATION_INLINE const Array<char>& byte_code() const
      { return _byte_code; }

    private:
      Array<char> _byte_code;
  };
} // butane

#endif // _BUTANE_SCRIPT_RESOURCE_H_
