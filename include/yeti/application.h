/*===-- yeti/application.h ----------------------------------*- mode: C -*-===*\
|*                                                                            *|
|*                             __ __     _   _                                *|
|*                            |  |  |___| |_|_|                               *|
|*                            |_   _| -_|  _| |                               *|
|*                              |_| |___|_| |_|                               *|
|*                                                                            *|
|*       This file is distributed under the terms described in LICENSE.       *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|*                                                                            *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef _YETI_APPLICATION_H_
#define _YETI_APPLICATION_H_

#include "yeti/common.h"

YETI_BEGIN_EXTERN_C /* { */

/*! \copydoc yeti_application */
typedef struct yeti_application yeti_application_t;

/*! ... */
typedef yeti_bool_t (*yeti_application_init_fn)(struct yeti_application *app);
/*! ... */
typedef void (*yeti_application_shutdown_fn)(struct yeti_application *app);
/*! ... */
typedef void (*yeti_application_update_fn)(struct yeti_application *app,
                                           const yeti_real32_t delta_time);
/*! ... */
typedef void (*yeti_application_render_fn)(struct yeti_application const *app);

/*! */
struct yeti_application {
  /*! \copydoc yeti_application_init_fn */
  yeti_application_init_fn init;
  /*! \copydoc yeti_application_shutdown_fn */
  yeti_application_shutdown_fn shutdown;
  /*! \copydoc yeti_application_update_fn */
  yeti_application_update_fn update;
  /*! \copydoc yeti_application_render_fn */
  yeti_application_render_fn render;
};

/*! ... */
extern YETI_PUBLIC
yeti_bool_t yeti_application_init(struct yeti_application *app);

/*! ... */
extern YETI_PUBLIC
void yeti_application_shutdown(struct yeti_application *app);

/*! ... */
extern YETI_PUBLIC
void yeti_application_update(struct yeti_application *app,
                             const yeti_real32_t delta_time);

/*! ... */
extern YETI_PUBLIC
void yeti_application_render(struct yeti_application const *app);

/*! ... */
extern YETI_PUBLIC
void yeti_application_pause(struct yeti_application *app);

/*! ... */
extern YETI_PUBLIC
void yeti_application_unpause(struct yeti_application *app);

/*! ... */
extern YETI_PUBLIC
void yeti_application_quit(struct yeti_application *app);

/*! ... */
extern YETI_PUBLIC
void yeti_application_start(struct yeti_application *app);

YETI_END_EXTERN_C /* } */

#endif /* _YETI_APPLICATION_H_ */

/*============================================================================*/
