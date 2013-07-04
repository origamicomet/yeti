// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCENE_GRAPH_MESH_H_
#define _BUTANE_SCENE_GRAPH_MESH_H_

/*! */
class Mesh final {
  __foundation_trait(Mesh, non_copyable);

  public:
    /*! */
    struct Serialized final {
      Resource::Id resource;
      uint32_t material;
    };

    /*! */
    struct VisualRepresentation final
      : public Node::VisualRepresentation
    {
      Resource::Handle<MeshResource> resource;
      size_t material;
    };

  public:
    Mesh(
      const Serialized& serialized );

    ~Mesh();

  public:
    /*! */
    void update_visual_representation(
      VisualRepresentation& vr ) const;

  public:
    FOUNDATION_INLINE Node& node()
    { return *((Node*)((uintptr_t)this - offsetof(Node, _as))); }

    FOUNDATION_INLINE const Node& node() const
    { return *((const Node*)((uintptr_t)this - offsetof(Node, _as))); }

    FOUNDATION_INLINE const Resource::Handle<MeshResource>& resource() const
    { return _resource; }

    FOUNDATION_INLINE const size_t material() const
    { return _material; }

    bool set_material(
      const MeshResource::Material::Name& name );

  private:
    Resource::Handle<MeshResource> _resource;
    size_t _material;
};

#endif // _BUTANE_SCENE_GRAPH_MESH_H_
