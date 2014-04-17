//===-- bitbyte/butane/foundation.h -----------------------------*- C++ -*-===//
//
//  Butane, a data-driven game engine.
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mwilliams@bitbyte.ca>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Includes Foundation.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_FOUNDATION_H_
#define _BITBYTE_BUTANE_FOUNDATION_H_

//===----------------------------------------------------------------------===//

#include <bitbyte/foundation/foundation.h>

/// \namespace bitbyte::butane::foundation
/// \copydoc bitbyte::foundation
///
namespace bitbyte {
namespace butane {
  namespace foundation = ::bitbyte::foundation;
} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

/// \def bitbyte_butane_assert
/// \copydoc bitbyte_foundation_assert
///
#define bitbyte_butane_assert(_Level, ...) \
  bitbyte_butane_assert_##_Level(__VA_ARGS__)

/// \def bitbyte_butane_assertf
/// \copydoc bitbyte_foundation_assertf
///
#define bitbyte_butane_assertf(_Level, ...) \
  bitbyte_butane_assertf_##_Level(__VA_ARGS__)

//===----------------------------------------------------------------------===//

#define bitbyte_butane_assert_always(_Message) \
  bitbyte_foundation_assert_always(_Message)

#define bitbyte_butane_assertf_always(_Format, ...) \
  bitbyte_foundation_assertf_always(_Format, ##__VA_ARGS__)

#ifdef BITBYTE_BUTANE_PARANOID
  #define bitbyte_butane_assert_paranoid(_Predicate) \
    bitbyte_foundation_assert_(_Predicate)
  #define bitbyte_butane_assertf_paranoid(_Predicate, _Format, ...) \
    bitbyte_foundation_assertf_(_Predicate, _Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_assert_paranoid(_Predicate) \
    do { (void)sizeof(_Predicate); } while (0)
  #define bitbyte_butane_assertf_paranoid(_Predicate, _Format, ...) \
    do { (void)sizeof(_Predicate); } while (0)
#endif

#if (BITBYTE_BUTANE_CONFIGURATION >= BITBYTE_BUTANE_DEBUG)
  #define bitbyte_butane_assert_debug(_Predicate) \
    bitbyte_foundation_assert_(_Predicate)
  #define bitbyte_butane_assertf_debug(_Predicate, _Format, ...) \
    bitbyte_foundation_assertf_(_Predicate, _Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_assert_debug(_Predicate) \
    do { (void)sizeof(_Predicate); } while (0)
  #define bitbyte_butane_assertf_debug(_Predicate, _Format, ...) \
    do { (void)sizeof(_Predicate); } while (0)
#endif

#if (BITBYTE_BUTANE_CONFIGURATION >= BITBYTE_BUTANE_DEVELOPMENT)
  #define bitbyte_butane_assert_development(_Predicate) \
    bitbyte_foundation_assert_(_Predicate)
  #define bitbyte_butane_assertf_development(_Predicate, _Format, ...) \
    bitbyte_foundation_assertf_(_Predicate, _Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_assert_development(_Predicate) \
    do { (void)sizeof(_Predicate); } while (0)
  #define bitbyte_butane_assertf_development(_Predicate, _Format, ...) \
    do { (void)sizeof(_Predicate); } while (0)
#endif

#if (BITBYTE_BUTANE_CONFIGURATION >= BITBYTE_BUTANE_RELEASE)
  #define bitbyte_butane_assert_release(_Predicate) \
    bitbyte_foundation_assert_(_Predicate)
  #define bitbyte_butane_assertf_release(_Predicate, _Format, ...) \
    bitbyte_foundation_assertf_(_Predicate, _Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_assert_release(_Predicate) \
    do { (void)sizeof(_Predicate); } while (0)
  #define bitbyte_butane_assertf_release(_Predicate, _Format, ...) \
    do { (void)sizeof(_Predicate); } while (0)
#endif

//===----------------------------------------------------------------------===//

/// \def bitbyte_butane_log
/// \copydoc bitbyte_foundation_log
///
#define bitbyte_butane_log(_Level, ...) \
  bitbyte_butane_log_##_Level(__VA_ARGS__)

/// \def bitbyte_butane_logf
/// \copydoc bitbyte_foundation_logf
///
#define bitbyte_butane_logf(_Level, ...) \
  bitbyte_butane_logf_##_Level(__VA_ARGS__)

//===----------------------------------------------------------------------===//

#define bitbyte_butane_log_always(_Message) \
  bitbyte_foundation_log_(_Message)

#define bitbyte_butane_logf_always(_Format, ...) \
  bitbyte_foundation_logf_(_Format, ##__VA_ARGS__)

#ifdef BITBYTE_BUTANE_PARANOID
  #define bitbyte_butane_log_paranoid(_Message) \
    bitbyte_foundation_log_(_Message)
  #define bitbyte_butane_logf_paranoid(_Format, ...) \
    bitbyte_foundation_logf_(_Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_log_paranoid(_Message) do {} while (0)
  #define bitbyte_butane_logf_paranoid(_Format, ...) do {} while (0)
#endif

#if (BITBYTE_BUTANE_CONFIGURATION >= BITBYTE_BUTANE_DEBUG)
  #define bitbyte_butane_log_debug(_Message) \
    bitbyte_foundation_log_(_Message)
  #define bitbyte_butane_logf_debug(_Format, ...) \
    bitbyte_foundation_logf_(_Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_log_debug(_Message) do {} while (0)
  #define bitbyte_butane_logf_debug(_Format, ...) do {} while (0)
#endif

#if (BITBYTE_BUTANE_CONFIGURATION >= BITBYTE_BUTANE_DEVELOPMENT)
  #define bitbyte_butane_log_development(_Message) \
    bitbyte_foundation_log_(_Message)
  #define bitbyte_butane_logf_development(_Format, ...) \
    bitbyte_foundation_logf_(_Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_log_development(_Message) do {} while (0)
  #define bitbyte_butane_logf_development(_Format, ...) do {} while (0)
#endif

#if (BITBYTE_BUTANE_CONFIGURATION >= BITBYTE_BUTANE_RELEASE)
  #define bitbyte_butane_log_release(_Message) \
    bitbyte_foundation_log_(_Message)
  #define bitbyte_butane_logf_release(_Format, ...) \
    bitbyte_foundation_logf_(_Format, ##__VA_ARGS__)
#else
  #define bitbyte_butane_log_release(_Message) do {} while (0)
  #define bitbyte_butane_logf_release(_Format, ...) do {} while (0)
#endif

//===----------------------------------------------------------------------===//

/// \def bitbyte_butane_new
/// \brief Placement new prettifier.
///
#define bitbyte_butane_new(_Allocator, _Type) \
  new ((_Allocator).alloc(sizeof(_Type), alignof(_Type))) _Type

/// \def bitbyte_butane_delete
/// \brief Placement delete prettifier.
///
#define bitbyte_butane_delete(_Allocator, _Type, _Ptr) \
  do { if (_Ptr) { (_Ptr)->~_Type(); _Allocator.free((void *)_Ptr); } } while (0)

//===----------------------------------------------------------------------===//

#endif // _BITBYTE_BUTANE_FOUNDATION_H_

//===----------------------------------------------------------------------===//
