/* ========================================================================== */
/*                                                                            */
/* This file is part of Butane.                                               */
/*                                                                            */
/* Author(s):                                                                 */
/*                                                                            */
/*   Michael Williams <devbug@bitbyte.ca>                                     */
/*                                                                            */
/* See LICENSE.md for licensing information.                                  */
/*                                                                            */
/* ========================================================================== */
/*! @file include/butane/time_step_policy.h
     ...                                                                      */
/* ========================================================================== */

#ifndef _BUTANE_TIME_STEP_POLICY_H_
#define _BUTANE_TIME_STEP_POLICY_H_

#include <butane/config.h>
#include <butane/foundation.h>
#include <butane/log.h>

/* ========================================================================== */
/*  Time-step Policy:                                                         */
/*   * C (butane_time_step_policy_t);                                         */
/*   * C++ (butane::TimeStepPolicy).                                          */
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_time_step_policy_t):                                            */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */

/*! ... */
typedef void (*butane_time_step_policy_update_fn)(
  struct butane_time_step_policy *time_step_policy,
  const fnd_monotonic_clock_t *wall,
  const fnd_monotonic_clock_t *frame);

/*! ... */
typedef void (*butane_time_step_policy_destroy_fn)(
  struct butane_time_step_policy *time_step_policy);

/*! ... */
typedef struct butane_time_step_policy {
  /* ======================================================================== */
  butane_time_step_policy_update_fn update;
  butane_time_step_policy_destroy_fn destroy;
  /* ======================================================================== */
  size_t num_of_steps_;
  float delta_time_per_step_;
  /* ======================================================================== */
} butane_time_step_policy_t;

/* ========================================================================== */

/*! ... */
extern BUTANE_API size_t butane_time_step_policy_num_of_steps(
  const butane_time_step_policy_t *time_step_policy);

/*! ... */
extern BUTANE_API float butane_time_step_policy_delta_time_per_step(
  const butane_time_step_policy_t *time_step_policy);

/* ========================================================================== */

/*! ... */
extern BUTANE_API butane_time_step_policy_t *butane_time_step_policy_variable(void);

/* ========================================================================== */

/*! ... */
extern BUTANE_API void butane_time_step_policy_update(
  butane_time_step_policy_t *time_step_policy,
  const fnd_monotonic_clock_t *wall,
  const fnd_monotonic_clock_t *frame);

/*! ... */
extern BUTANE_API void butane_time_step_policy_destroy(
  butane_time_step_policy_t *time_step_policy);

/* ========================================================================== */

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::TimeStepPolicy):                                             */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /*! @copydoc butane_time_step_policy_t */
  class BUTANE_API TimeStepPolicy : private ::butane_time_step_policy_t {
    private:
      TimeStepPolicy();
      TimeStepPolicy(const TimeStepPolicy &);
      TimeStepPolicy& operator= (const TimeStepPolicy &);
      ~TimeStepPolicy();
    public: /* private: */
      static TimeStepPolicy *recover_(::butane_time_step_policy_t *tsp);
      ::butane_time_step_policy_t *lose_();
      static const TimeStepPolicy *recover_(const ::butane_time_step_policy_t *tsp);
      const ::butane_time_step_policy_t *lose_() const;
    public:
      /*! @copydoc butane_time_step_policy_num_of_steps */
      size_t num_of_steps() const;
      /*! @copydoc butane_time_step_policy_delta_time_per_step */
      float delta_time_per_step() const;
    public:
      /*! @copydoc butane_time_step_policy_variable */
      static TimeStepPolicy *variable();
      /*! @copydoc butane_time_step_policy_t::update */
      void update(
        const foundation::clock::Monotonic &wall,
        const foundation::clock::Monotonic &frame);
      /*! @copydoc butane_time_step_policy_t::destroy */
      void destroy();
  };
} /* butane */
#endif

/* ========================================================================== */

#endif /* _BUTANE_TIME_STEP_POLICY_H_ */
