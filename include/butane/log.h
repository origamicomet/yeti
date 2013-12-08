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
/*! @file include/butane/window.h
     ...                                                                      */
/* ========================================================================== */

#ifndef _BUTANE_LOG_H_
#define _BUTANE_LOG_H_

#include <butane/config.h>
#include <butane/foundation.h>

/* ========================================================================== */
/*  Logging:                                                                  */
/*   * C (butane_log, butane_log_level, and butane_set_log_level);            */
/*   * C++ (butane::log, butane::log_level, and butane::set_log_level).       */
/* ========================================================================== */

#include <stdarg.h>

/* ========================================================================== */
/*  C (butane_log, butane_log_level, and butane_set_log_level):               */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */

/*! ... */
typedef enum {
  /*! Nothing. */
  BUTANE_LOG_NOTHING = 0,
  /*! Non-recoverable errors. */
  BUTANE_LOG_ERROR = 1,
  /*! Warning messages. */
  BUTANE_LOG_WARNING = 2,
  /*! Normal messages. */
  BUTANE_LOG_INFO = 3,
  /*! Useful debug information. */
  BUTANE_LOG_DEBUG = 4,
  /*! Logs everything. */
  BUTANE_LOG_SPEW = 5
} butane_log_level_t;

/* ========================================================================== */

/*! ... */
extern BUTANE_API butane_log_level_t butane_log_level(void);

/*! ... */
extern BUTANE_API void butane_set_log_level(const butane_log_level_t level);

/* ========================================================================== */

/*! ... */
extern BUTANE_API void butane_log(
  const butane_log_level_t level,
  const char *message);

/*! ... */
extern BUTANE_API void butane_logf(
  const butane_log_level_t level,
  const char *format, ...);

/*! ... */
extern BUTANE_API void butane_logfv(
  const butane_log_level_t level,
  const char *format,
  va_list va);

/* ========================================================================== */

/*! ... */
extern BUTANE_API void butane_fail(const char *reason);

/*! ... */
extern BUTANE_API void butane_failf(const char *format, ...);

/*! ... */
extern BUTANE_API void butane_failfv(const char *format, va_list va);

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

  namespace LogLevel {
    enum _ {
      /*! @copydoc BUTANE_LOG_NOTHING */
      Nothing = ::BUTANE_LOG_NOTHING,
      /*! @copydoc BUTANE_LOG_ERROR */
      Error = ::BUTANE_LOG_ERROR,
      /*! @copydoc BUTANE_LOG_WARNING */
      Warning = ::BUTANE_LOG_WARNING,
      /*! @copydoc BUTANE_LOG_INFO */
      Info = ::BUTANE_LOG_INFO,
      /*! @copydoc BUTANE_LOG_DEBUG */
      Debug = ::BUTANE_LOG_DEBUG,
      /*! @copydoc BUTANE_LOG_SPEW */
      Spew = ::BUTANE_LOG_SPEW
    };
  }

  /*! @copydoc butane_log_level */
  typedef LogLevel::_ LogLevel_;

  /*========================================================================= */

  /*! @copydoc butane_log_level */
  extern BUTANE_API LogLevel_ log_level();

  /*! @copydoc butane_set_log_level */
  extern BUTANE_API void set_log_level(const LogLevel_ level);

  /*========================================================================= */

  /*! @copydoc butane_log */
  extern BUTANE_API void log(const LogLevel_ level, const char *message);

  /*! @copydoc butane_logf */
  extern BUTANE_API void logf(const LogLevel_ level, const char *format, ...);

  /*! @copydoc butane_logfv */
  extern BUTANE_API void logfv(const LogLevel_ level, const char *format, va_list va);

  /*========================================================================= */

  /*! @copydoc butane_fail */
  extern BUTANE_API void fail(const char *reason);

  /*! @copydoc butane_failf */
  extern BUTANE_API void failf(const char *format, ...);

  /*! @copydoc butane_failfv */
  extern BUTANE_API void failfv(const char *format, va_list va);

  /*========================================================================= */
} /* butane */
#endif

/* ========================================================================== */

#endif /* _BUTANE_LOG_H_ */
