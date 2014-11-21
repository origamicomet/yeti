//===-- bitbyte/butane/time_step_policy.h -----------------------*- C++ -*-===//
//
//  Butane
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@bitbyte.ca>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Provides a standard interface for various time step policies.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_TIME_STEP_POLICY_H_
#define _BITBYTE_BUTANE_TIME_STEP_POLICY_H_

//============================================================================//

#include "bitbyte/butane/config.h"
#include "bitbyte/butane/foundation.h"
#include "bitbyte/butane/linkage.h"

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

/// \brief
///
typedef enum bitbyte_butane_time_step_policy_type {
  /// Unknown.
  BITBYTE_BUTANE_TIME_STEP_POLICY_UNKNOWN = 0,
  /// Variable.
  BITBYTE_BUTANE_TIME_STEP_POLICY_VARIABLE = 1,
  /// Fixed.
  BITBYTE_BUTANE_TIME_STEP_POLICY_FIXED = 2
} bitbyte_butane_time_step_policy_type_t;

//===----------------------------------------------------------------------===//

/// \brief
///
typedef struct bitbyte_butane_time_step_policy_opts {
  /// \brief
  bitbyte_butane_time_step_policy_type_t type;
  /// \brief
  union {
    struct {
    } variable;
    struct {
      /// \brief
      float delta_time_per_frame;
    } fixed;
  };
} bitbyte_butane_time_step_policy_opts_t;

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_time_step_policy_opts_t
  typedef bitbyte_butane_time_step_policy_opts_t butane_time_step_policy_opts_t;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
typedef struct bitbyte_butane_time_step_policy {
  /// \brief
  bitbyte_butane_time_step_policy_opts_t opts;
  /// \brief
  size_t steps;
  /// \brief
  float delta_time_per_step;
} bitbyte_butane_time_step_policy_t;

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_time_step_policy_t
  typedef bitbyte_butane_time_step_policy_t butane_time_step_policy_t;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
bitbyte_butane_time_step_policy_t *
bitbyte_butane_time_step_policy_create(
  const bitbyte_butane_time_step_policy_opts_t *opts);

/// \def bitbyte_butane_time_step_policy_create
/// \copydoc bitbyte_butane_time_step_policy_create
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_time_step_policy_create bitbyte_butane_time_step_policy_create
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_time_step_policy_update(
  bitbyte_butane_time_step_policy_t *time_step_policy,
  const bitbyte_foundation_timer_t *frame,
  const bitbyte_foundation_timer_t *wall);

/// \def bitbyte_butane_time_step_policy_update
/// \copydoc bitbyte_butane_time_step_policy_update
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_time_step_policy_update bitbyte_butane_time_step_policy_update
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_time_step_policy_destroy(
  bitbyte_butane_time_step_policy_t *time_step_policy);

/// \def bitbyte_butane_time_step_policy_destroy
/// \copydoc bitbyte_butane_time_step_policy_destroy
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_time_step_policy_destroy bitbyte_butane_time_step_policy_destroy
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

#ifdef __cplusplus
}
#endif // __cplusplus

//============================================================================//

#ifdef __cplusplus

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//

/// \copydoc ::bitbyte_butane_time_step_policy_t
class BITBYTE_BUTANE_EXPORT TimeStepPolicy
  : public bitbyte_butane_time_step_policy_t
{
 public:
  /// \copydoc ::bitbyte_butane_time_step_policy_type_t
  typedef ::bitbyte_butane_time_step_policy_type_t Type;
  /// \copydoc ::bitbyte_butane_time_step_policy_opts_t
  typedef ::bitbyte_butane_time_step_policy_opts_t Options;

  /// \copydoc BITBYTE_BUTANE_TIME_STEP_POLICY_UNKNOWN
  static const Type Unknown = BITBYTE_BUTANE_TIME_STEP_POLICY_UNKNOWN;
  /// \copydoc BITBYTE_BUTANE_TIME_STEP_POLICY_VARIABLE
  static const Type Variable = BITBYTE_BUTANE_TIME_STEP_POLICY_VARIABLE;
  /// \copydoc BITBYTE_BUTANE_TIME_STEP_POLICY_FIXED
  static const Type Fixed = BITBYTE_BUTANE_TIME_STEP_POLICY_FIXED;

 protected:
  TimeStepPolicy();
  TimeStepPolicy(const TimeStepPolicy &time_step_policy);
  TimeStepPolicy &operator=(const TimeStepPolicy &time_step_policy);
  ~TimeStepPolicy();

 public:
  /// \copydoc ::bitbyte_butane_time_step_policy_create
  static TimeStepPolicy *create(
    const TimeStepPolicy::Options &options);

  /// \copydoc ::bitbyte_butane_time_step_policy_update
  void update(
    const bitbyte::foundation::Timer &frame,
    const bitbyte::foundation::Timer &wall);

  /// \copydoc ::bitbyte_butane_time_step_policy_destroy
  void destroy(void);
};

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//

#endif // _BITBYTE_BUTANE_TIME_STEP_POLICY_H_

//============================================================================//
