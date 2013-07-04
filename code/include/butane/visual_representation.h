// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VISUAL_REPRESENTATION_H_
#define _BUTANE_VISUAL_REPRESENTATION_H_

#include <butane/butane.h>

namespace butane {
  class VisualRepresentation {
    public:
      /*! */
      typedef uint64_t Id;

      /*! */
      static const Id invalid = (Id)0xFFFFFFFFFFFFFFFFull;

      /*! */
      enum Type {
        INVALID = 0xFFFFFFFFu,
        /*! A SceneGraph::Node or derivative. */
        NODE    = (1u << 31u),
        /*! A SceneGraph::Node::Camera. */
        CAMERA  = 1u | NODE,
        /*! A SceneGraph::Node::Mesh. */
        MESH    = 2u | NODE
      };

    public:
      VisualRepresentation();

      ~VisualRepresentation();

    public:
      /*! */
      static VisualRepresentation::Type type(
        const VisualRepresentation::Id id );

      /*! */
      static size_t length(
        const VisualRepresentation::Type type );

    public:
      FOUNDATION_INLINE Type type() const
      { return VisualRepresentation::type(id); }

      FOUNDATION_INLINE size_t length() const
      { return VisualRepresentation::length(type()); }

    public:
      VisualRepresentation::Id id;
  };
} // butane

#endif // _BUTANE_VISUAL_REPRESENTATION_H_
