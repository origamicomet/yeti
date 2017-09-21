//===-- yeti/core/debug/assert.h ------------------------*- mode: C++11 -*-===//
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
/// \brief Custom (and more sophisticated) assertions.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_DEBUG_ASSERT_H_
#define _YETI_CORE_DEBUG_ASSERT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// Describes an assertion.
struct Assertion {
  /// The predicate.
  ///
  /// \note May be `NULL`. When `NULL` a reason will be given.
  ///
  const char *predicate;

  /// A custom, more friendly description of why the predicate failed.
  ///
  /// \note May be `NULL`.
  ///
  const char *reason;

  /// Describes where the assertion is in the codebase.
  struct {
    const char *file;
    size_t line;
  } location;
};

/// A first-chance assertion handler.
typedef void (*AssertionHandler)(const Assertion &assertion, void *context);

/// Gets the first-chance assertion handler.
extern YETI_PUBLIC void assertion_handler(AssertionHandler *assertion_handler,
                                          void **context);

/// Sets the first-chance assertion handler.
extern YETI_PUBLIC void set_assertion_handler(AssertionHandler assertion_handler,
                                              void *context = NULL);

/// \internal Raises @assertion for the first-chance assertion handler to handle.
extern YETI_PUBLIC void raise_an_assertion(const Assertion &assertion);

} // core
} // yeti

/// \def yeti_assert
///
/// \brief Raises a fuss if @Predicate doesn't evaluate to truthy.
///
#define yeti_assert(Predicate)                                                \
  do {                                                                        \
    if (!(Predicate)) {                                                       \
      ::yeti::core::Assertion assertion;                                      \
                                                                              \
      assertion.predicate = #Predicate;                                       \
      assertion.reason = NULL;                                                \
      assertion.location.file = __FILE__ ;                                    \
      assertion.location.line = (size_t)__LINE__;                             \
                                                                              \
      ::yeti::core::raise_an_assertion(assertion);                            \
                                                                              \
                                                                              \
      YETI_TRAP();                                                            \
    }                                                                         \
  } while (0,0)                                                               \

/// \def yeti_assert_with_reason
///
/// \brief Raises a fuss if @Predicate doesn't evaluate to thruthy, providing
/// user-specified, formatted reason.
///
#define yeti_assert_with_reason(Predicate, Format, ...)                       \
  do {                                                                        \
    if (!(Predicate)) {                                                       \
      ::yeti::core::Assertion assertion;                                      \
                                                                              \
      assertion.predicate = #Predicate;                                       \
                                                                              \
      const size_t length = snprintf(NULL, 0, Format, ##__VA_ARGS__) + 1;     \
      char *reason = (char *)alloca(length);                                  \
                                                                              \
      snprintf(reason, length, Format, ##__VA_ARGS__);                        \
                                                                              \
      assertion.reason = (const char *)reason;                                \
                                                                              \
      assertion.location.file = __FILE__ ;                                    \
      assertion.location.line = (size_t)__LINE__;                             \
                                                                              \
      ::yeti::core::raise_an_assertion(assertion);                            \
                                                                              \
                                                                              \
      YETI_TRAP();                                                            \
    }                                                                         \
  } while (0,0)

#if defined(DOXYGEN)
  /// \copydoc yeti_assert
  /// @{
  #define yeti_assert_debug yeti_assert
  #define yeti_assert_development yeti_assert
  #define yeti_assert_release yeti_assert
  /// @}

  /// \copydoc yeti_assert_with_reason
  /// @{
  #define yeti_assert_with_reason_debug yeti_assert_with_reason
  #define yeti_assert_with_reason_development yeti_assert_with_reason
  #define yeti_assert_with_reason_release yeti_assert_with_reason
  /// @}
#else
  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
    #define yeti_assert_debug yeti_assert
    #define yeti_assert_with_reason_debug yeti_assert_with_reason
  #else
    #define yeti_assert_debug(...)
    #define yeti_assert_with_reason_debug(...)
  #endif

  #if YETI_CONFIGURATION <= YETI_CONFIGURATION_DEVELOPMENT
    #define yeti_assert_development yeti_assert
    #define yeti_assert_with_reason_development yeti_assert_with_reason
  #else
    #define yeti_assert_development(...)
    #define yeti_assert_with_reason_development(...)
  #endif

  #if YETI_CONFIGURATION <= YETI_CONFIGURATION_RELEASE
    #define yeti_assert_release yeti_assert
    #define yeti_assert_with_reason_release yeti_assert_with_reason
  #else
    #define yeti_assert_release(...)
    #define yeti_assert_with_reason_release(...)
  #endif
#endif

#endif // _YETI_CORE_DEBUG_ASSERT_H_
