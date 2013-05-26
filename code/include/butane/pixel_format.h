// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_PIXEL_FORMAT_H_
#define _BUTANE_PIXEL_FORMAT_H_

#include <butane/foundation.h>
#include <butane/config.h>

namespace butane {
  class BUTANE_EXPORT PixelFormat final {
    public:
      static const uint32_t ColorBit      = (1u << 31u);
      static const uint32_t DepthBit      = (1u << 30u);
      static const uint32_t StencilBit    = (1u << 29u);
      static const uint32_t CompressedBit = (1u << 28u);

      enum Value {
        INVALID =
          0x00000000u,

        R8G8B8A8 =
          0x00000001u | ColorBit,

        R8G8B8A8_SRGB =
          0x00000002u | ColorBit,

        DXT1 =
          0x00000003u | ColorBit | CompressedBit,

        DXT3 =
          0x00000004u | ColorBit | CompressedBit,

        DXT5 =
          0x00000005u | ColorBit | CompressedBit,

        D24S8 =
          0x00000006u | DepthBit | StencilBit | CompressedBit
      };

    public:
      PixelFormat(
        const Value value = INVALID
      ) : _value(value)
      {}

      PixelFormat(
        const PixelFormat& pf
      ) : _value(pf._value)
      {}

      PixelFormat& operator= (
        const PixelFormat& pf )
      { _value = pf._value; return *this; }

    public:
      FOUNDATION_INLINE operator uint32_t() const
      { return _value; }

    public:
      size_t row_stride(
        const uint32_t width ) const;

      size_t layer_stride(
        const uint32_t width,
        const uint32_t height ) const;

      size_t storage_requirements(
        const uint32_t width,
        const uint32_t height,
        const uint32_t depth ) const;

    public:
      FOUNDATION_INLINE bool is_color() const
      { return ((_value & ColorBit) == ColorBit); }

      FOUNDATION_INLINE bool is_depth() const
      { return ((_value & DepthBit) == DepthBit); }

      FOUNDATION_INLINE bool is_stencil() const
      { return ((_value & StencilBit) == StencilBit); }

      FOUNDATION_INLINE bool is_compressed() const
      { return ((_value & CompressedBit) == CompressedBit); }

    public:
      FOUNDATION_INLINE uint32_t& value()
      { return _value; }

      FOUNDATION_INLINE const uint32_t& value() const
      { return _value; }

    private:
      uint32_t _value;
  };
} // butane

#endif // _BUTANE_PIXEL_FORMAT_H_
