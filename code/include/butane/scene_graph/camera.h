// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCENE_GRAPH_CAMERA_H_
#define _BUTANE_SCENE_GRAPH_CAMERA_H_

/*! */
class Camera final {
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

  public:
    Camera(
      const Serialized& serialized );

    Camera(
      const Camera& camera );

    Camera& operator= (
      const Camera& camera );

    ~Camera();

  private:
    Type _type;
    Mat4 _projection;
};

#endif // _BUTANE_SCENE_GRAPH_CAMERA_H_
