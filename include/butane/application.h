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
/*! @file include/butane/application.h
     ...                                                                      */
/* ========================================================================== */

#ifndef _BUTANE_APPLICATION_H_
#define _BUTANE_APPLICATION_H_

#include <butane/config.h>
#include <butane/foundation.h>
#include <butane/log.h>

/* ========================================================================== */
/*  Application:                                                              */
/*   * C (butane_application_t);                                              */
/*   * C++ (butane::Application).                                             */
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_application_t):                                                 */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */

/*! ... */
typedef bool (*butane_application_initialize_fn)(
  struct butane_application *application);

/*! ... */
typedef void (*butane_application_update_fn)(
  struct butane_application *application,
  const float delta_time);

/*! ... */
typedef void (*butane_application_render_fn)(
  const struct butane_application *application);

/*! ... */
typedef void (*butane_application_shutdown_fn)(
  struct butane_application *application);

/*! ... */
typedef struct butane_application {
  /* ======================================================================== */
  butane_application_initialize_fn initialize;
  butane_application_update_fn update;
  butane_application_render_fn render;
  butane_application_shutdown_fn shutdown;
  /* ======================================================================== */
  struct butane_time_step_policy *time_step_policy_;
  /* ======================================================================== */
} butane_application_t;

/* ========================================================================== */

/*! ... */
extern BUTANE_API const char *butane_application_platform(
  const butane_application_t *app);

/*! ... */
extern BUTANE_API const char *butane_application_architecture(
  const butane_application_t *app);

/*! ... */
extern BUTANE_API const char *butane_application_build(
  const butane_application_t *app);

/*! ... */
extern BUTANE_API struct butane_time_step_policy *butane_application_time_step_policy(
  const butane_application_t *app);

/*! ... */
extern BUTANE_API void butane_application_set_time_step_policy(
  butane_application_t *app,
  struct butane_time_step_policy *time_step_policy);

/* ========================================================================== */

/*! ... */
extern BUTANE_API bool butane_application_initialize(
  butane_application_t *app);

/*! ... */
extern BUTANE_API void butane_application_update(
  butane_application_t *app,
  const float delta_time);

/*! ... */
extern BUTANE_API void butane_application_render(
  const butane_application_t *app);

/*! ... */
extern BUTANE_API void butane_application_shutdown(
  butane_application_t *app);

/* ========================================================================== */

/*! ... */
extern BUTANE_API void butane_application_run(
  butane_application_t *app);

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::Application):                                                */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  class TimeStepPolicy;
  /*! @copydoc butane_application_t */
  class BUTANE_API Application {
    private:
      Application(const Application &);
      Application& operator= (const Application &);
    protected:
      Application();
      virtual ~Application();
    public: /* private: */
      static Application *recover_(::butane_application_t *app);
      ::butane_application_t *lose_();
      static const Application *recover_(const ::butane_application_t *app);
      const ::butane_application_t *lose_() const;
    public:
      /*! @copydoc butane_application_platform */
      const char *platform() const;
      /*! @copydoc butane_application_architecture */
      const char *architecture() const;
      /*! @copydoc butane_application_build */
      const char *build() const;
      /*! @copydoc butane_application_time_step_policy */
      TimeStepPolicy *time_step_policy() const;
      /*! @copydoc butane_application_set_time_step_policy */
      void set_time_step_policy(TimeStepPolicy *time_step_policy);
    public:
      /*! @copydoc butane_application_t::initialize */
      virtual bool initialize();
      /*! @copydoc butane_application_t::update */
      virtual void update(const float delta_time);
      /*! @copydoc butane_application_t::render */
      virtual void render() const;
      /*! @copydoc butane_application_t::shutdown */
      virtual void shutdown();
    public:
      /*! @copydoc butane_application_run */
      void run();
    private:
      static bool initialize_(::butane_application_t *app);
      static void update_(::butane_application_t *app, const float delta_time);
      static void render_(const ::butane_application_t *app);
      static void shutdown_(::butane_application_t *app);
    private:
      ::butane_application_t _;
      Application *this_;
  };
} /* butane */
#endif

/* ========================================================================== */

#endif /* _BUTANE_APPLICATION_H_ */
