//===-- yeti/core/utilities.h ---------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
/// \file
/// \brief Miscellaneous utilities.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_UTILITIES_H_
#define _YETI_CORE_UTILITIES_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// \namespace ::yeti::core::utility
/// \brief Miscellaneous utilities.
namespace utility {
  /// \brief Returns the lesser of @a and @b.
  template <typename T>
  static YETI_INLINE T min(const T &a, const T &b) { return (a < b) ? a : b; }

  /// \brief Returns the greater of @a and @b.
  template <typename T>
  static YETI_INLINE T max(const T &a, const T &b) { return (a > b) ? a : b; }

  /// \brief Returns @v clamped to @min and @max, inclusive.
  template <typename T>
  static YETI_INLINE T clamp(const T &v, const T &min, const T &max) { return min(max(min, v), max); }

  /// \brief Quickly determines if @v is a power of two.
  /// @{
  static YETI_INLINE bool is_power_of_two(u8 v) { return v && !(v & (v - 1)); }
  static YETI_INLINE bool is_power_of_two(u16 v) { return v && !(v & (v - 1)); }
  static YETI_INLINE bool is_power_of_two(u32 v) { return v && !(v & (v - 1)); }
  static YETI_INLINE bool is_power_of_two(u64 v) { return v && !(v & (v - 1)); }
  /// @}

  /// \brief Quickly determines if @ptr is a power of two.
  static YETI_INLINE bool is_power_of_two(void *ptr) {
    return ptr && !(uintptr_t(ptr) & (uintptr_t(ptr) - 1));
  }
}

} // core
} // yeti

/// \def YETI_MIN
/// \copydoc ::yeti::core::utility::min
#define YETI_MIN(a, b) \
  (::yeti::core::utility::min(a, b))

/// \def YETI_MAX
/// \copydoc ::yeti::core::utility::max
#define YETI_MAX(a, b) \
  (::yeti::core::utility::max(a, b))

/// \def YETI_CLAMP
/// \copydoc ::yeti::core::utility::clamp
#define YETI_CLAMP(v, min, max) \
  (::yeti::core::utility::clamp(v, min, max))

/// \def YETI_IS_POWER_OF_TWO
/// \copydoc ::yeti::core::utility::is_power_of_two
#define YETI_IS_POWER_OF_TWO(v) \
  (::yeti::core::utility::is_power_of_two(v))

// TODO(mtwilliams): Use templates to improve safety of `YETI_ELEMENTS_IN_ARRAY`.

/// \def YETI_ELEMENTS_IN_ARRAY
/// \brief Calculates the number of elements in an @array.
#define YETI_ELEMENTS_IN_ARRAY(array) \
  (sizeof(array) / sizeof(array[0]))

#endif // _YETI_CORE_UTILITIES_H_
