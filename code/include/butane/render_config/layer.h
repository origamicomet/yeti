// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

/*! */
class BUTANE_EXPORT Layer {
  public:
    /*! */
    typedef uint8_t Id;

    /*! */
    typedef Hash<uint32_t, murmur_hash> Name;

  public:
    /*! */
    Layer::Id id;

    /*! */
    Layer::Name name;

    /*! */
    size_t num_of_samplers;

    /*! */
    Resource::Id samplers[BT_LAYER_MAXIMUM_NUM_OF_SAMPLERS];

    /*! */
    size_t num_of_render_targets;

    /*! */
    Resource::Id render_targets[BT_LAYER_MAXIMUM_NUM_OF_RENDER_TARGETS];

    /*! */
    Resource::Id depth_stencil_target;

    // /*! */
    // Generator::Id generator;
};
