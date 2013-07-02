// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCENE_GRAPH_MESH_H_
#define _BUTANE_SCENE_GRAPH_MESH_H_

/*! */
class Mesh final {
  public:
    /*! */
    struct Serialized final {
      Resource::Id resource;
      uint32_t material;
    };

  public:
    Mesh(
      const Serialized& serialized );

    Mesh(
      const Mesh& mesh );

    Mesh& operator= (
      const Mesh& mesh );

    ~Mesh();

  public:
    FOUNDATION_INLINE const Resource::Handle<MeshResource>& resource() const
    { return _resource; }

    FOUNDATION_INLINE const uint material() const
    { return _material; }

  private:
    Resource::Handle<MeshResource> _resource;
    uint _material;
};

#endif // _BUTANE_SCENE_GRAPH_MESH_H_
