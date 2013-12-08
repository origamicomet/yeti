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
 #  include <butane/log.h>
/* ========================================================================== */

#include <stdlib.h>
#include <stdio.h>

/* ========================================================================== */
/*  C (butane_log, butane_log_level, and butane_set_log_level):               */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */

static butane_log_level_t butane_log_level_ = BUTANE_LOG_NOTHING;

butane_log_level_t butane_log_level(void) {
  return butane_log_level_;
}

void butane_set_log_level(const butane_log_level_t level) {
  butane_assert(debug, level >= BUTANE_LOG_NOTHING);
  butane_assert(debug, level <= BUTANE_LOG_SPEW);
  butane_log_level_ = level;
}

/* ========================================================================== */

void butane_log(
  const butane_log_level_t level,
  const char *message)
{
  butane_assert(debug, level >= BUTANE_LOG_NOTHING);
  butane_assert(debug, level <= BUTANE_LOG_SPEW);
  butane_assert(debug, message != NULL);
  if (level > butane_log_level_)
    return;
  /* TODO(mtwilliams): Use a logger interface. */
  fprintf(stdout, message);
}

void butane_logf(
  const butane_log_level_t level,
  const char *format, ...)
{
  va_list va;
  va_start(va, format);
  butane_logfv(level, format, va);
  va_end(va);
}

void butane_logfv(
  const butane_log_level_t level,
  const char *format,
  va_list va)
{
  butane_assert(debug, level >= BUTANE_LOG_NOTHING);
  butane_assert(debug, level <= BUTANE_LOG_SPEW);
  butane_assert(debug, format != NULL);
  if (level > butane_log_level_)
    return;
  /* TODO(mtwilliams): Use a logger interface. */
  vfprintf(stdout, format, va);
}

/* ========================================================================== */

void butane_fail(const char *reason) {
  butane_assert(debug, reason != NULL);
  butane_log(BUTANE_LOG_ERROR, reason);
  exit(EXIT_FAILURE);
}

void butane_failf(const char *format, ...) {
  butane_assert(debug, format != NULL);
  va_list va;
  va_start(va, format);
  butane_failfv(format, va);
  va_end(va);
}

void butane_failfv(const char *format, va_list va) {
  butane_assert(debug, format != NULL);
  butane_logfv(BUTANE_LOG_ERROR, format, va);
  exit(EXIT_FAILURE);
}

/* ========================================================================== */

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::log, butane::log_level, and butane::set_log_level):          */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /*========================================================================= */

  LogLevel_ log_level() {
    return ((LogLevel_)::butane_log_level());
  }

  void set_log_level(const LogLevel_ level) {
    ::butane_set_log_level(((::butane_log_level_t)level));
  }

  /*========================================================================= */

  void log(const LogLevel_ level, const char *message) {
    ::butane_log(((::butane_log_level_t)level), message);
  }

  void logf(const LogLevel_ level, const char *format, ...) {
    va_list va;
    va_start(va, format);
    ::butane_logfv(((::butane_log_level_t)level), format, va);
    va_end(va);
  }

  void logfv(const LogLevel_ level, const char *format, va_list va) {
    ::butane_logfv(((::butane_log_level_t)level), format, va);
  }

  /*========================================================================= */

  void fail(const char *reason) {
    ::butane_fail(reason);
  }

  void failf(const char *format, ...) {
    va_list va;
    va_start(va, format);
    ::butane_failfv(format, va);
    va_end(va);
  }

  void failfv(const char *format, va_list va) {
    ::butane_failfv(format, va);
  }

  /*========================================================================= */
} /* butane */
#endif

/* ========================================================================== */
