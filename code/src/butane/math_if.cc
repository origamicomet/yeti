// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/math_if.h>
#include <butane/math.h>

namespace butane {
namespace Math {
  struct Temporary {
    private:
      static Allocator& __allocator_initializer();
      static const thread_safe::Static< Allocator > __ts_allocator;

    public:
      static Allocator& allocator();

    public:
      FOUNDATION_INLINE Vec2f& vec2()
      { return *((Vec2f*)&as.vec2[0]); }

      FOUNDATION_INLINE Vec3f& vec3()
      { return *((Vec3f*)&as.vec3[0]); }

      FOUNDATION_INLINE Vec4f& vec4()
      { return *((Vec4f*)&as.vec4[0]); }

      FOUNDATION_INLINE Quatf& quat()
      { return *((Quatf*)&as.quat[0]); }

    private:
      union {
        uint8_t vec2[sizeof(Vec2f)];
        uint8_t vec3[sizeof(Vec3f)];
        uint8_t vec4[sizeof(Vec4f)];
        uint8_t quat[sizeof(Quatf)];
      } as;
  };

  Allocator& Temporary::__allocator_initializer() {
    static ProxyAllocator allocator("math temporaries", Allocators::heap());
    return allocator;
  }

  const thread_safe::Static< Allocator >
    Temporary::__ts_allocator(__allocator_initializer);

  Allocator& Temporary::allocator() {
    return __ts_allocator();
  }
} // Math
} // butane

namespace butane {
  namespace script_interface {
    static Array<Math::Temporary>& temporaries(
      const Script& script )
    {
      void* temporaries; {
        script.get("__math_if__temporaries", temporaries); }
      return *((Array<Math::Temporary>*)temporaries);
    }

    static Math::Temporary* temporary(
      const Script& script )
    {
      if (temporaries(script).size() == temporaries(script).reserved())
        script.error("Reached maximum number of temporaries (%u).", BT_SCRIPT_MAXIMUM_NUM_OF_TEMPORARY_MATH_OBJECTS);
      return &temporaries(script)[temporaries(script).size()];
    }

    namespace Vec2 {
      static void* ctor(
        Script& script,
        const Script::Arguments& arguments )
      {
        float x, y;
        switch ((size_t)arguments) {
          case 0:
            x = y = 0.0f;
            break;
          case 2:
            arguments.to(0, x);
            arguments.to(1, y);
            break;
          default:
            script.error("Expected zero or two arguments ([x : Number, y : Number]).");
        }

        Math::Temporary* v = temporary(script);
        v->vec2().x = x;
        v->vec2().y = y;
        return (void*)v;
      }

      static void dtor(
        Script& script,
        void* self )
      {
      }

      static size_t add(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (addend : Vec2).");
        Math::Temporary* addend;
        arguments.to(0, "Vec2", (void*&)addend);
        Math::Temporary* sum = temporary(script);
        sum->vec2() = ((Math::Temporary*)self)->vec2() + addend->vec2();
        script.stack().push("Vec2", (void*)sum);
        return 1;
      }

      static size_t sub(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (subtrahend : Vec2).");
        Math::Temporary* subtrahend;
        arguments.to(0, "Vec2", (void*&)subtrahend);
        Math::Temporary* difference = temporary(script);
        difference->vec2() = ((Math::Temporary*)self)->vec2() - subtrahend->vec2();
        script.stack().push("Vec2", (void*)difference);
        return 1;
      }

      static size_t mul(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (factor : Vec2).");
        Math::Temporary* factor;
        arguments.to(0, "Vec2", (void*&)factor);
        Math::Temporary* product = temporary(script);
        product->vec2() = ((Math::Temporary*)self)->vec2() * factor->vec2();
        script.stack().push("Vec2", (void*)product);
        return 1;
      }

      static size_t div(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (divisor : Vec2).");
        Math::Temporary* divisor;
        arguments.to(0, "Vec2", (void*&)divisor);
        Math::Temporary* quotient = temporary(script);
        quotient->vec2() = ((Math::Temporary*)self)->vec2() / divisor->vec2();
        script.stack().push("Vec2", (void*)quotient);
        return 1;
      }

      static size_t length(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec2().length());
        return 1;
      }

      static size_t magnitude(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec2().magnitude());
        return 1;
      }

      static size_t dot(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (term : Vec2).");
        Math::Temporary* term;
        arguments.to(0, "Vec2", (void*&)term);
        script.stack().push(((Math::Temporary*)self)->vec2().dot(term->vec2()));
        return 1;
      }

      static size_t normalize(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 0)
          script.error("Expected no arguments.");
        Math::Temporary* normalized = temporary(script);
        normalized->vec2() = ((Math::Temporary*)self)->vec2().normalize();
        script.stack().push("Vec2", (void*)normalized);
        return 1;
      }

      static size_t x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec2().x);
        return 1;
      }

      static size_t set_x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec2().x);
        return 1;
      }

      static size_t y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec2().y);
        return 1;
      }

      static size_t set_y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec2().y);
        return 1;
      }
    } // Vec2

    namespace Vec3 {
      static void* ctor(
        Script& script,
        const Script::Arguments& arguments )
      {
        float x, y, z;
        switch ((size_t)arguments) {
          case 0:
            x = y = z = 0.0f;
            break;
          case 3:
            arguments.to(0, x);
            arguments.to(1, y);
            arguments.to(2, z);
            break;
          default:
            script.error("Expected zero or three arguments ([x : Number, y : Number, z : Number]).");
        }

        Math::Temporary* v = temporary(script);
        v->vec3().x = x;
        v->vec3().y = y;
        v->vec3().z = z;
        return (void*)v;
      }

      static void dtor(
        Script& script,
        void* self )
      {
      }

      static size_t add(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (addend : Vec3).");
        Math::Temporary* addend;
        arguments.to(0, "Vec3", (void*&)addend);
        Math::Temporary* sum = temporary(script);
        sum->vec3() = ((Math::Temporary*)self)->vec3() + addend->vec3();
        script.stack().push("Vec3", (void*)sum);
        return 1;
      }

      static size_t sub(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (subtrahend : Vec3).");
        Math::Temporary* subtrahend;
        arguments.to(0, "Vec3", (void*&)subtrahend);
        Math::Temporary* difference = temporary(script);
        difference->vec3() = ((Math::Temporary*)self)->vec3() - subtrahend->vec3();
        script.stack().push("Vec3", (void*)difference);
        return 1;
      }

      static size_t mul(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (factor : Vec3).");
        Math::Temporary* factor;
        arguments.to(0, "Vec3", (void*&)factor);
        Math::Temporary* product = temporary(script);
        product->vec3() = ((Math::Temporary*)self)->vec3() * factor->vec3();
        script.stack().push("Vec3", (void*)product);
        return 1;
      }

      static size_t div(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (divisor : Vec3).");
        Math::Temporary* divisor;
        arguments.to(0, "Vec3", (void*&)divisor);
        Math::Temporary* quotient = temporary(script);
        quotient->vec3() = ((Math::Temporary*)self)->vec3() / divisor->vec3();
        script.stack().push("Vec3", (void*)quotient);
        return 1;
      }

      static size_t length(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec3().length());
        return 1;
      }

      static size_t magnitude(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec3().magnitude());
        return 1;
      }

      static size_t dot(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (term : Vec3).");
        Math::Temporary* term;
        arguments.to(0, "Vec3", (void*&)term);
        script.stack().push(((Math::Temporary*)self)->vec3().dot(term->vec3()));
        return 1;
      }

      static size_t cross(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (with : Vec3)");
        Math::Temporary* with;
        arguments.to(0, "Vec3", (void*&)with);
        Math::Temporary* cross_product = temporary(script);
        cross_product->vec3() = ((Math::Temporary*)self)->vec3().cross(with->vec3());
        script.stack().push("Vec3", (void*)cross_product);
        return 1;
      }

      static size_t normalize(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 0)
          script.error("Expected no arguments.");
        Math::Temporary* normalized = temporary(script);
        normalized->vec3() = ((Math::Temporary*)self)->vec3().normalize();
        script.stack().push("Vec3", (void*)normalized);
        return 1;
      }

      static size_t x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec3().x);
        return 1;
      }

      static size_t set_x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec3().x);
        return 1;
      }

      static size_t y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec3().y);
        return 1;
      }

      static size_t set_y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec3().y);
        return 1;
      }

      static size_t z(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec3().z);
        return 1;
      }

      static size_t set_z(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec3().z);
        return 1;
      }
    } // Vec3

    namespace Vec4 {
      static void* ctor(
        Script& script,
        const Script::Arguments& arguments )
      {
        float x, y, z, w;
        switch ((size_t)arguments) {
          case 0:
            x = y = z = w = 0.0f;
            break;
          case 4:
            arguments.to(0, x);
            arguments.to(1, y);
            arguments.to(2, z);
            arguments.to(3, w);
            break;
          default:
            script.error("Expected zero or four arguments ([x : Number, y : Number, z : Number, w : Number]).");
        }

        Math::Temporary* v = temporary(script);
        v->vec4().x = x;
        v->vec4().y = y;
        v->vec4().z = z;
        v->vec4().w = w;
        return (void*)v;
      }

      static void dtor(
        Script& script,
        void* self )
      {
      }

      static size_t add(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (addend : Vec4).");
        Math::Temporary* addend;
        arguments.to(0, "Vec4", (void*&)addend);
        Math::Temporary* sum = temporary(script);
        sum->vec4() = ((Math::Temporary*)self)->vec4() + addend->vec4();
        script.stack().push("Vec4", (void*)sum);
        return 1;
      }

      static size_t sub(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (subtrahend : Vec4).");
        Math::Temporary* subtrahend;
        arguments.to(0, "Vec4", (void*&)subtrahend);
        Math::Temporary* difference = temporary(script);
        difference->vec4() = ((Math::Temporary*)self)->vec4() - subtrahend->vec4();
        script.stack().push("Vec4", (void*)difference);
        return 1;
      }

      static size_t mul(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (factor : Vec4).");
        Math::Temporary* factor;
        arguments.to(0, "Vec4", (void*&)factor);
        Math::Temporary* product = temporary(script);
        product->vec4() = ((Math::Temporary*)self)->vec4() * factor->vec4();
        script.stack().push("Vec4", (void*)product);
        return 1;
      }

      static size_t div(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (divisor : Vec4).");
        Math::Temporary* divisor;
        arguments.to(0, "Vec4", (void*&)divisor);
        Math::Temporary* quotient = temporary(script);
        quotient->vec4() = ((Math::Temporary*)self)->vec4() / divisor->vec4();
        script.stack().push("Vec4", (void*)quotient);
        return 1;
      }

      static size_t length(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec4().length());
        return 1;
      }

      static size_t magnitude(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec4().magnitude());
        return 1;
      }

      static size_t dot(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (term : Vec4).");
        Math::Temporary* term;
        arguments.to(0, "Vec4", (void*&)term);
        script.stack().push(((Math::Temporary*)self)->vec4().dot(term->vec4()));
        return 1;
      }

      static size_t normalize(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 0)
          script.error("Expected no arguments.");
        Math::Temporary* normalized = temporary(script);
        normalized->vec4() = ((Math::Temporary*)self)->vec4().normalize();
        script.stack().push("Vec4", (void*)normalized);
        return 1;
      }

      static size_t x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec4().x);
        return 1;
      }

      static size_t set_x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec4().x);
        return 1;
      }

      static size_t y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec4().y);
        return 1;
      }

      static size_t set_y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec4().y);
        return 1;
      }

      static size_t z(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec4().z);
        return 1;
      }

      static size_t set_z(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec4().z);
        return 1;
      }

      static size_t w(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->vec4().w);
        return 1;
      }

      static size_t set_w(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->vec4().w);
        return 1;
      }
    } // Vec4

    namespace Quat {
      static void* ctor(
        Script& script,
        const Script::Arguments& arguments )
      {
        float w, x, y, z;
        switch ((size_t)arguments) {
          case 0:
            w = 1.0f;
            x = y = z = 0.0f;
            break;
          case 4:
            arguments.to(0, x);
            arguments.to(1, y);
            arguments.to(2, z);
            arguments.to(3, w);
            break;
          default:
            script.error("Expected zero or four arguments ([w : Number, x : Number, y : Number, z : Number]).");
        }

        Math::Temporary* q = temporary(script);
        q->quat().w = w;
        q->quat().x = x;
        q->quat().y = y;
        q->quat().z = z;
        return (void*)q;
      }

      static void dtor(
        Script& script,
        void* self )
      {
      }

      static size_t from_axis_angle(
        Script& script,
        const Script::Arguments& arguments )
      {
        if (arguments != 2)
          script.error("Expected two arguments (axis : Vec3, angle_in_radians : Number).");
        Math::Temporary* axis;
        arguments.to(0, "Quat", (void*&)axis);
        float angle_in_radians;
        arguments.to(1, angle_in_radians);
        Math::Temporary* q = temporary(script);
        q->quat() = Quatf::from_axis_angle(axis->vec3(), angle_in_radians);
        script.stack().push("Quat", (void*)q);
        return 1;
      }

      static size_t from_euler_angles(
        Script& script,
        const Script::Arguments& arguments )
      {
        if (arguments != 2)
          script.error("Expected three arguments (x_in_radians : Number, y_in_radians : Number, z_in_radians : Number).");
        float x_in_radians, y_in_radians, z_in_radians;
        arguments.to(0, x_in_radians);
        arguments.to(1, y_in_radians);
        arguments.to(2, z_in_radians);
        Math::Temporary* q = temporary(script);
        q->quat() = Quatf::from_euler_angles(x_in_radians, y_in_radians, z_in_radians);
        script.stack().push("Quat", (void*)q);
        return 1;
      }

      static size_t add(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (addend : Quat).");
        Math::Temporary* addend;
        arguments.to(0, "Quat", (void*&)addend);
        Math::Temporary* sum = temporary(script);
        sum->quat() = ((Math::Temporary*)self)->quat() + addend->quat();
        script.stack().push("Quat", (void*)sum);
        return 1;
      }

      static size_t sub(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (subtrahend : Quat).");
        Math::Temporary* subtrahend;
        arguments.to(0, "Quat", (void*&)subtrahend);
        Math::Temporary* difference = temporary(script);
        difference->quat() = ((Math::Temporary*)self)->quat() - subtrahend->quat();
        script.stack().push("Quat", (void*)difference);
        return 1;
      }

      static size_t mul(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (factor : Quat).");
        Math::Temporary* factor;
        arguments.to(0, "Quat", (void*&)factor);
        Math::Temporary* product = temporary(script);
        product->quat() = ((Math::Temporary*)self)->quat() * factor->quat();
        script.stack().push("Quat", (void*)product);
        return 1;
      }

      static size_t div(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (divisor : Quat).");
        Math::Temporary* divisor;
        arguments.to(0, "Quat", (void*&)divisor);
        Math::Temporary* quotient = temporary(script);
        quotient->quat() = ((Math::Temporary*)self)->quat() / divisor->quat();
        script.stack().push("Quat", (void*)quotient);
        return 1;
      }

      static size_t length(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->quat().length());
        return 1;
      }

      static size_t magnitude(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->quat().magnitude());
        return 1;
      }

      static size_t dot(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 1)
          script.error("Expected one argument (term : Quat).");
        Math::Temporary* term;
        arguments.to(0, "Quat", (void*&)term);
        script.stack().push(((Math::Temporary*)self)->quat().dot(term->quat()));
        return 1;
      }

      static size_t normalize(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 0)
          script.error("Expected no arguments.");
        Math::Temporary* normalized = temporary(script);
        normalized->quat() = ((Math::Temporary*)self)->quat().normalize();
        script.stack().push("Quat", (void*)normalized);
        return 1;
      }

      static size_t inverse(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        if (arguments != 0)
          script.error("Expected no arguments.");
        Math::Temporary* inversed = temporary(script);
        inversed->quat() = ((Math::Temporary*)self)->quat().inverse();
        script.stack().push("Quat", (void*)inversed);
        return 1;
      }

      static size_t w(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->quat().w);
        return 1;
      }

      static size_t set_w(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->quat().w);
        return 1;
      }

      static size_t x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->quat().x);
        return 1;
      }

      static size_t set_x(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->quat().x);
        return 1;
      }

      static size_t y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->quat().y);
        return 1;
      }

      static size_t set_y(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->quat().y);
        return 1;
      }

      static size_t z(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        script.stack().push(((Math::Temporary*)self)->quat().z);
        return 1;
      }

      static size_t set_z(
        Script& script,
        void* self,
        const Script::Arguments& arguments )
      {
        arguments.to(0, ((Math::Temporary*)self)->quat().z);
        return 1;
      }
    } // Quat

    static void on_destruction(
      const Script& script )
    {
      void* temporaries; {
        script.get("__math_if__temporaries", temporaries); }
      make_delete(Array, Math::Temporary::allocator(), ((Array<Math::Temporary>*)temporaries));
    }
  } // script_interface
} // butane

namespace butane {
namespace Math {
  void expose(
    butane::Script& script )
  {
    script.set(
      "__math_if__temporaries",
      (void*)(make_new(Array<Math::Temporary>, Math::Temporary::allocator())(
        Math::Temporary::allocator(),
        BT_SCRIPT_MAXIMUM_NUM_OF_TEMPORARY_MATH_OBJECTS)));

    script.on_destruction(&script_interface::on_destruction);

    script.type(&script_interface::Vec2::ctor, &script_interface::Vec2::dtor)
      .operation("add", &script_interface::Vec2::add)
      .operation("sub", &script_interface::Vec2::sub)
      .operation("mul", &script_interface::Vec2::mul)
      .operation("div", &script_interface::Vec2::div)
      .getter("length", &script_interface::Vec2::length)
      .getter("magnitude", &script_interface::Vec2::magnitude)
      .method("dot", &script_interface::Vec2::dot)
      .method("normalize", &script_interface::Vec2::normalize)
      .accessors("x", &script_interface::Vec2::x, &script_interface::Vec2::set_x)
      .accessors("y", &script_interface::Vec2::y, &script_interface::Vec2::set_y)
    .expose("Vec2");

    script.type(&script_interface::Vec3::ctor, &script_interface::Vec3::dtor)
      .operation("add", &script_interface::Vec3::add)
      .operation("sub", &script_interface::Vec3::sub)
      .operation("mul", &script_interface::Vec3::mul)
      .operation("div", &script_interface::Vec3::div)
      .getter("length", &script_interface::Vec3::length)
      .getter("magnitude", &script_interface::Vec3::magnitude)
      .method("dot", &script_interface::Vec3::dot)
      .method("cross", &script_interface::Vec3::cross)
      .method("normalize", &script_interface::Vec3::normalize)
      .accessors("x", &script_interface::Vec3::x, &script_interface::Vec3::set_x)
      .accessors("y", &script_interface::Vec3::y, &script_interface::Vec3::set_y)
      .accessors("z", &script_interface::Vec3::z, &script_interface::Vec3::set_z)
    .expose("Vec3");

    script.type(&script_interface::Vec4::ctor, &script_interface::Vec4::dtor)
      .operation("add", &script_interface::Vec4::add)
      .operation("sub", &script_interface::Vec4::sub)
      .operation("mul", &script_interface::Vec4::mul)
      .operation("div", &script_interface::Vec4::div)
      .getter("length", &script_interface::Vec4::length)
      .getter("magnitude", &script_interface::Vec4::magnitude)
      .method("dot", &script_interface::Vec4::dot)
      .method("normalize", &script_interface::Vec4::normalize)
      .accessors("x", &script_interface::Vec4::x, &script_interface::Vec4::set_x)
      .accessors("y", &script_interface::Vec4::y, &script_interface::Vec4::set_y)
      .accessors("z", &script_interface::Vec4::z, &script_interface::Vec4::set_z)
      .accessors("w", &script_interface::Vec4::w, &script_interface::Vec4::set_w)
    .expose("Vec4");

    script.type(&script_interface::Quat::ctor, &script_interface::Quat::dtor)
      .method("from_axis_angle", &script_interface::Quat::from_axis_angle)
      .method("from_euler_angles", &script_interface::Quat::from_euler_angles)
      .operation("add", &script_interface::Quat::add)
      .operation("sub", &script_interface::Quat::sub)
      .operation("mul", &script_interface::Quat::mul)
      .operation("div", &script_interface::Quat::div)
      .getter("length", &script_interface::Quat::length)
      .getter("magnitude", &script_interface::Quat::magnitude)
      .method("dot", &script_interface::Quat::dot)
      .method("normalize", &script_interface::Quat::normalize)
      .method("inverse", &script_interface::Quat::inverse)
      .accessors("w", &script_interface::Quat::w, &script_interface::Quat::set_w)
      .accessors("x", &script_interface::Quat::x, &script_interface::Quat::set_x)
      .accessors("y", &script_interface::Quat::y, &script_interface::Quat::set_y)
      .accessors("z", &script_interface::Quat::z, &script_interface::Quat::set_z)
    .expose("Quat");

  }
} // Math
} // butane
