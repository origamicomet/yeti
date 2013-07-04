// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCENE_GRAPH_CAMERA_H_
#define _BUTANE_SCENE_GRAPH_CAMERA_H_

/*! */
class Camera final {
  __foundation_trait(Camera, non_copyable);

  public:
    /*! */
    enum Type {
      /*! Orthographic projection (no foreshortening). */
      ORTHOGRAPHIC = 1,
      /*! Perspective projection (foreshortening). */
      PERSPECTIVE  = 2
    };

  public:
    /*! */
    struct Serialized final {
      Camera::Type type;
      float near, far;
      union {
        struct {
          float min_x, max_x;
          float min_y, max_y;
        } orthographic;
        struct {
          float field_of_view;
          float aspect_ratio;
        } perspective;
      };
    };

    /*! */
    struct VisualRepresentation final
      : public Node::VisualRepresentation
    {
      Mat4 projection;
    };

  public:
    Camera(
      const Serialized& serialized );

    ~Camera();

  public:
    /*! */
    void update_visual_representation(
      VisualRepresentation& vr ) const;

  public:
    FOUNDATION_INLINE Node& node()
    { return *((Node*)((uintptr_t)this - offsetof(Node, _as))); }

    FOUNDATION_INLINE const Node& node() const
    { return *((const Node*)((uintptr_t)this - offsetof(Node, _as))); }

    FOUNDATION_INLINE Camera::Type type() const
    { return _type; }

    void set_type(
      const Camera::Type type );

    FOUNDATION_INLINE float near() const
    { return _near; }

    void set_near(
      const float near );

    FOUNDATION_INLINE float far() const
    { return _far; }

    void set_far(
      const float far );

    FOUNDATION_INLINE void view( Vec2f& min, Vec2f& max ) const {
      min = Vec2f(_as.orthographic.min_x, _as.orthographic.min_y);
      max = Vec2f(_as.orthographic.max_x, _as.orthographic.max_y);
    }

    void set_view(
      const Vec2f& min,
      const Vec2f& max );

    FOUNDATION_INLINE float field_of_view() const
    { return _as.perspective.field_of_view; }

    void set_field_of_view(
      const float field_of_view );

    FOUNDATION_INLINE float aspect_ratio() const
    { return _as.perspective.aspect_ratio; }

    void set_aspect_ratio(
      const float aspect_ratio );

  private:
    Camera::Type _type;
    float _near, _far;
    union {
      struct {
        float min_x, max_x;
        float min_y, max_y;
      } orthographic;
      struct {
        float field_of_view;
        float aspect_ratio;
      } perspective;
    } _as;
};

#endif // _BUTANE_SCENE_GRAPH_CAMERA_H_
