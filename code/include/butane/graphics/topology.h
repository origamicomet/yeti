// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TOPOLOGY_H_
#define _BUTANE_TOPOLOGY_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Topology final {
    public:
      enum Value {
        INVALID   = 0,
        TRIANGLES = 1
      };

    public:
      Topology(
        const Value value = INVALID
      ) : _value(value)
      {}

      Topology(
        const Topology& topology
      ) : _value(topology._value)
      {}

      Topology& operator= (
        const Topology& topology )
      { _value = topology._value; return *this; }

    public:
      FOUNDATION_INLINE operator Value() const
      { return _value; }

    public:
      FOUNDATION_INLINE Value& value()
      { return _value; }

      FOUNDATION_INLINE const Value& value() const
      { return _value; }

    private:
      Value _value;
  };
} // butane

#endif // _BUTANE_TOPOLOGY_H_
