// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

/*! */
class BUTANE_EXPORT Resource {
  public:
    /*! */
    typedef uint32_t Id;

    /*! */
    typedef Hash<uint32_t, murmur_hash> Name;

    /*! */
    enum Type {
      /*! */
      RENDER_TARGET        = 1u,
      /*! */
      DEPTH_STENCIL_TARGET = 2u,
    };

  public:
    /*! */
    static const Id special = (Id)(1ull << ((sizeof(Id) * 8) - 1));

    /*! */
    static const Id invalid = (Id)0xFFFFFFFFFFFFFFFFull;

    /*! */
    class Special {
      __foundation_trait(Special, non_constructable);
      __foundation_trait(Special, non_copyable);

      public:
        /*! */
        static const Resource::Id back_buffer = Resource::special | 1ull;
    };

  public:
    /*! */
    Resource::Id id;

    /*! */
    Resource::Name name;

    /*! */
    Resource::Type type;

    /*! */
    union {
      struct {
        struct {
          float x, y;
        } scale;
        PixelFormat::Value format;
        uint8_t clear;
      } render_or_depth_stencil_target;
    };
};
