//===-- yeti/components/camera.cc -----------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/components/camera.h"

namespace yeti {

// TODO(mtwilliams): Error messages during compilation.
// TODO(mtwilliams): Contextualize by indicating location in source.

#pragma pack(push, 1)
  struct CompiledCamera {
    u32 type;

    union {
      struct {
        f32 field_of_view;
      } perspective;

      struct {
        f32 top;
        f32 left;
        f32 bottom;
        f32 right;
      } orthographic;
    };

    f32 near;
    f32 far;
  };
#pragma pack(pop)

class CameraCompiler {
 public:
  CameraCompiler(const component_compiler::Environment *env,
                 const component_compiler::Input *input,
                 const component_compiler::Output *output);

  ~CameraCompiler();

 public:
  bool compile();

 private:
  bool parse();
  bool validate();
  bool output();

 private:
  bool typecheck(Camera::Type expected) const;

 private:
  bool parse_field_of_view(const xml_element_t *e);
  bool parse_top(const xml_element_t *e);
  bool parse_left(const xml_element_t *e);
  bool parse_bottom(const xml_element_t *e);
  bool parse_right(const xml_element_t *e);
  bool parse_near(const xml_element_t *e);
  bool parse_far(const xml_element_t *e);

 private:
  bool extract(const xml_element_t *e, f32 *v);

 private:
  const component_compiler::Environment *env_;
  const component_compiler::Input *input_;
  const component_compiler::Output *output_;

  // Tracks whether or not various properties have been specified.
  bool type_has_been_specified_,
       top_has_been_specified_,
       left_has_been_specified_,
       bottom_has_been_specified_,
       right_has_been_specified_,
       near_has_been_specified_,
       far_has_been_specified_,
       field_of_view_has_been_specified_;

  // Intermediate storage of the camera we're compiling. Eventually written
  // out after compilation.
  CompiledCamera camera_;
};

CameraCompiler::CameraCompiler(const component_compiler::Environment *env,
                               const component_compiler::Input *input,
                               const component_compiler::Output *output)
  : env_(env)
  , input_(input)
  , output_(output)
  , type_has_been_specified_(false)
  , top_has_been_specified_(false)
  , left_has_been_specified_(false)
  , bottom_has_been_specified_(false)
  , right_has_been_specified_(false)
  , near_has_been_specified_(false)
  , far_has_been_specified_(false)
  , field_of_view_has_been_specified_(false)
{
  core::memory::zero((void *)&camera_, sizeof(CompiledCamera));
}

CameraCompiler::~CameraCompiler()
{
}

bool CameraCompiler::compile()
{
  return parse() && validate() && output();
}

bool CameraCompiler::parse()
{
  const xml_element_t *root = input_->root;

  // We parse the component in entirety so we can report as many issues as
  // possible to reduce frustration.
  unsigned errors = 0;

  for (const xml_element_t *e = root->children; e; e = e->sibling) {
    if (core::string::compare("type", e->name.s, e->name.l)) {
      if (type_has_been_specified_) {
        env_->error(env_, "Type has already been specified!");
        errors++;
      } else {
        if (core::string::compare("perspective", e->body.s, e->body.l)) {
          camera_.type = (u32)Camera::PERSPECTIVE;
        } else if (core::string::compare("orthographic", e->body.s, e->body.l)) {
          camera_.type = (u32)Camera::ORTHOGRAPHIC;
        } else {
          env_->error(env_, "Cameras can only be perspective or orthographic not \"%.*s\".", e->body.l, e->body.s);
          errors++;
        }
        type_has_been_specified_ = true;
      }
    } else {
      if (!type_has_been_specified_)
        env_->warning(env_, "Type of camera not specified before \"%.*s\".", e->name.l, e->name.l);

      if (core::string::compare("field_of_view", e->name.s, e->name.l)) {
        if (!this->parse_field_of_view(e))
          errors++;
      } else if (core::string::compare("top", e->name.s, e->name.l)) {
        if (!this->parse_top(e))
          errors++;
      } else if (core::string::compare("left", e->name.s, e->name.l)) {
        if (!this->parse_left(e))
          errors++;
      } else if (core::string::compare("bottom", e->name.s, e->name.l)) {
        if (!this->parse_bottom(e))
          errors++;
      } else if (core::string::compare("right", e->name.s, e->name.l)) {
        if (!this->parse_right(e))
          errors++;
      } else if (core::string::compare("near", e->name.s, e->name.l)) {
        if (!this->parse_near(e))
          errors++;
      } else if (core::string::compare("far", e->name.s, e->name.l)) {
        if (!this->parse_far(e))
          errors++;
      } else {
        env_->error(env_, "Cameras do not have a \"%.*s\" property.", e->name.l, e->name.s);
        errors++;
      }
    }
  }

  return (errors == 0);
}

bool CameraCompiler::typecheck(Camera::Type expected) const
{
  if (type_has_been_specified_)
    if (camera_.type != expected)
      return false;

  return true;
}

#define CHECK(Type, Name, Field) \
  if ((Type) && !this->typecheck(Type)) { \
    env_->error(env_, Name " cannot be specified for non-%.*s cameras!", camera::type_to_string(Type)); \
    return false; \
  } else { \
    if (Field##_has_been_specified_) { \
      env_->error(env_, Name " has already been specified!"); \
      return false; \
    } else { \
      Field##_has_been_specified_ = true; \
    } \
  }

// Indicates when we don't care about the type of camera.
#define _ Camera::UNKNOWN

bool CameraCompiler::parse_field_of_view(const xml_element_t *e) {
  CHECK(Camera::PERSPECTIVE, "Field-of-view", field_of_view);
  return this->extract(e, &camera_.perspective.field_of_view);
}

bool CameraCompiler::parse_top(const xml_element_t *e) {
  CHECK(Camera::ORTHOGRAPHIC, "Top", top);
  return this->extract(e, &camera_.orthographic.top);
}

bool CameraCompiler::parse_left(const xml_element_t *e) {
  CHECK(Camera::ORTHOGRAPHIC, "Left", left);
  return this->extract(e, &camera_.orthographic.left);
}

bool CameraCompiler::parse_bottom(const xml_element_t *e) {
  CHECK(Camera::ORTHOGRAPHIC, "Bottom", bottom);
  return this->extract(e, &camera_.orthographic.bottom);
}

bool CameraCompiler::parse_right(const xml_element_t *e) {
  CHECK(Camera::ORTHOGRAPHIC, "Right", right);
  return this->extract(e, &camera_.orthographic.right);
}

bool CameraCompiler::parse_near(const xml_element_t *e) {
  CHECK(_, "Near", near);
  return this->extract(e, &camera_.near);
}

bool CameraCompiler::parse_far(const xml_element_t *e) {
  CHECK(_, "Far", far);
  return this->extract(e, &camera_.far);
}

#undef CHECK
#undef _

bool CameraCompiler::extract(const xml_element_t *e, f32 *v)
{
  if (sscanf(e->body.s, "%f", v) != 1) {
    env_->error(env_, "Expected a floating-point value for \"%.*s\" but got \"%.*s\".", e->name.l, e->name.s, e->body.l, e->body.l);
    return false;
  }

  return true;
}

bool CameraCompiler::validate()
{
  if (!type_has_been_specified_) {
    env_->error(env_, "Type of camera not specified!");
    return false;
  }

  switch (camera_.type) {
    case Camera::PERSPECTIVE: {
      if (!field_of_view_has_been_specified_) {
        env_->error(env_, "Field-of-view not specified!");
        return false;
      } else if (camera_.perspective.field_of_view <= 0.f) {
        env_->error(env_, "Field-of-view must be greater than 0°!");
        return false;
      } else if (camera_.perspective.field_of_view > 180.f) {
        env_->error(env_, "Field-of-view must be less than 180°!");
        return false;
      }
    } break;

    case Camera::ORTHOGRAPHIC: {
      if (!top_has_been_specified_) {
        env_->error(env_, "Top not specified!");
        return false;
      } else if (!left_has_been_specified_) {
        env_->error(env_, "Left not specified!");
        return false;
      } else if (!bottom_has_been_specified_) {
        env_->error(env_, "Bottom not specified!");
        return false;
      } else if (!right_has_been_specified_) {
        env_->error(env_, "Right not specified!");
        return false;
      }
    } break;
  }

  if (!near_has_been_specified_) {
    env_->error(env_, "Near plane not specified!");
    return false;
  } else if (!far_has_been_specified_) {
    env_->error(env_, "Far plane not specified!");
    return false;
  }

  return true;
}

bool CameraCompiler::output()
{
  output_->write(env_, (const void *)&camera_, sizeof(CompiledCamera));
  return true;
}

namespace camera {
  Camera::Type type_from_string(const char *string) {
    if (core::string::compare("perspective", string))
      return Camera::PERSPECTIVE;
    else if (core::string::compare("orthographic", string))
      return Camera::ORTHOGRAPHIC;
    return Camera::UNKNOWN;
  }

  const char *type_to_string(Camera::Type type) {
    switch (type) {
      case Camera::PERSPECTIVE: return "perspective";
      case Camera::ORTHOGRAPHIC: return "orthographic";
    }

    return "unknown";
  }

  static bool compile(const component_compiler::Environment *env,
                      const component_compiler::Input *input,
                      const component_compiler::Output *output)
  {
    return CameraCompiler(env, input, output).compile();
  }

  static void spawn(void *system,
                    const Entity *entities,
                    const u32 *owners,
                    size_t instances,
                    const void *data)
  {
  }

  static bool compatible(u32 version)
  {
    return false;
  }
}

CameraSystem::CameraSystem(EntityManager *entities)
  : System(CameraSystem::component())
  , entities_(entities)
  , entity_to_instance_(core::global_heap_allocator(), 256)
  , instance_to_entity_(core::global_heap_allocator())
  , cameras_(core::global_heap_allocator())
  , siblings_(core::global_heap_allocator())
{
}

CameraSystem::~CameraSystem() {
}

// Automatically register with component registry.
YETI_AUTO_REGISTER_COMPONENT(CameraSystem::component());

const Component *CameraSystem::component() {
  static const Component::Property properties[] = {
    { "type",          Component::Property::STRING, 1 },
    { "field_of_view", Component::Property::FLOAT,  1 },
    { "box",           Component::Property::FLOAT,  4 },
    { "near",          Component::Property::FLOAT,  1 },
    { "far",           Component::Property::FLOAT,  1 }
  };

  static const Component component = {
    /* .name              = */ "camera",
    /* .version           = */ 1,
    /* .order             = */ 0,
    /* .properties        = */ properties,
    /* .num_of_properties = */ YETI_ELEMENTS_IN_ARRAY(properties),
    /* .create_a_system   = */ &CameraSystem::create,
    /* .destroy_a_system  = */ &CameraSystem::destroy,
    /* .compile           = */ &camera::compile,
    /* .spawn             = */ &camera::spawn,
    /* .compatible        = */ &camera::compatible
  };

  return &component;
}

void *CameraSystem::create(EntityManager *entities) {
  return YETI_NEW(CameraSystem, core::global_heap_allocator())(entities);
}

void CameraSystem::destroy(void *system) {
  YETI_DELETE(CameraSystem, core::global_heap_allocator(), (CameraSystem *)system);
}

} // yeti
