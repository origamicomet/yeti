//===-- yeti/foundation/assert.h --------------------------*- mode: C++ -*-===//
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
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_ASSERT_H_
#define _YETI_FOUNDATION_ASSERT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"

namespace yeti {
namespace foundation {

/// Describes a (failed) assertion.
struct Assertion {
  /// The predicate that failed.
  const char *predicate;
  /// A custom, more friendly description of why the predicate failed.
  const char *reason;
  /// Describes where the assertion is in the codebase.
  struct {
    const char *file;
    size_t line;
  } location;
};

///
typedef void (*AssertionHandler)(const Assertion &assertion, void *ctx);

///
extern YETI_PUBLIC
void assertion_handler(AssertionHandler *assertion_handler, void **ctx);

///
extern YETI_PUBLIC
void set_assertion_handler(AssertionHandler new_assertion_handler, void *ctx = NULL);

} // foundation
} // yeti

/// \def yeti_assert
/// \brief Raises a fuss if |_Predicate| doesn't evaluate to true.
///
#define yeti_assert(_Predicate) \
  do { \
    if (!(_Predicate)) { \
      ::yeti::foundation::Assertion assertion; \
      assertion.predicate = #_Predicate; \
      assertion.reason = NULL; \
      assertion.location.file = __FILE__ ; \
      assertion.location.line = (size_t)__LINE__; \
      ::yeti::foundation::AssertionHandler assertion_handler; \
      void *assertion_handler_ctx; \
      ::yeti::foundation::assertion_handler(&assertion_handler, &assertion_handler_ctx); \
      if (assertion_handler != NULL) \
        assertion_handler(assertion, assertion_handler_ctx); \
      YETI_TRAP(); \
    } \
  } while (0,0)

/// \def yeti_assertf
/// \brief Raises a fuss if @_Predicate doesn't evaluate to true, providing
/// user-specified, formatted reasoning.
///
#define yeti_assertf(_Predicate, _Reason_format, ...) \
  do { \
    if (!(_Predicate)) { \
      ::yeti::foundation::Assertion assertion; \
      assertion.predicate = #_Predicate; \
      const size_t reason_len = snprintf(NULL, 0, _Reason_format, ##__VA_ARGS__) + 1; \
      const char *reason = (const char *)alloca(reason_len); \
      snprintf((char *)reason, reason_len, _Reason_format, ##__VA_ARGS__); \
      assertion.reason = reason; \
      assertion.location.file = __FILE__; \
      assertion.location.line = (size_t)__LINE__; \
      ::yeti::foundation::AssertionHandler assertion_handler; \
      void *assertion_handler_ctx; \
      ::yeti::foundation::assertion_handler(&assertion_handler, &assertion_handler_ctx); \
      if (assertion_handler != NULL) \
        assertion_handler(assertion, assertion_handler_ctx); \
      YETI_TRAP(); \
    } \
  } while (0,0)

#if defined(DOXYGEN)
  #define yeti_assert_debug yeti_assert
  #define yeti_assert_development yeti_assert
  #define yeti_assert_release yeti_assert
#else
  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
    #define yeti_assert_debug yeti_assert
    #define yeti_assertf_debug yeti_assertf
  #else
    #define yeti_assert_debug(...)
    #define yeti_assertf_debug(...)
  #endif

  #if YETI_CONFIGURATION <= YETI_CONFIGURATION_DEVELOPMENT
    #define yeti_assert_development yeti_assert
    #define yeti_assertf_development yeti_assertf
  #else
    #define yeti_assert_development(...)
    #define yeti_assertf_development(...)
  #endif

  #if YETI_CONFIGURATION <= YETI_CONFIGURATION_RELEASE
    #define yeti_assert_release yeti_assert
    #define yeti_assertf_release yeti_assertf
  #else
    #define yeti_assert_release(...)
    #define yeti_assertf_release(...)
  #endif
#endif

#endif // _YETI_FOUNDATION_ASSERT_H_
