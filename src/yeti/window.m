//===-- yeti/window.m -----------------------------*- mode: Objective-C -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

// TODO(mtwilliams): Use `@autoreleasepool`?
// TODO(mtwilliams): Expose windowed, fullscreen, and fullscreen borderless.
// TODO(mtwilliams): Retina.

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

typedef struct yeti_cocoa_window {
  NSWindow *window;
  bool resizeable;
  bool closable;
} yeti_cocoa_window_t;

typedef struct yeti_cocoa_window_desc {
  const char *title;
  unsigned width;
  unsigned height;
} yeti_cocoa_window_desc_t;

@interface Window<NSWindow, NSWindowDelegate>
  @property(nonatomic, assign) yeti_cocoa_window_t *wrapper;

  - (BOOL)isFullscreen;
  - (BOOL)isBorderless;
@end

@implementation Window
  @synthesize wrapper;

  - (BOOL)isFullscreen {
    return !!([self styleMask] & NSFullScreenWindowMask);
  }

  - (BOOL)isBorderless {
    return !!([self styleMask] & NSBorderlessWindowMask);
  }

  - (BOOL)windowShouldClose:(id)sender {
    return wrapper->closable ? YES : NO;
  }

  - (void)windowWillClose:(NSNotification *)notification {
    (void)notification;

    NSNotificationCenter *notification_center =
      [NSNotificationCenter defaultCenter];

    [notification_center removeObserver:window name:NSWindowWillCloseNotification object:window];
    [notification_center removeObserver:window name:NSWindowDidResizeNotification object:window];
    // [notification_center removeObserver:window name:NSWindowDidExposeNotification object:window];
    // [notification_center removeObserver:window name:NSWindowDidBecomeKeyNotification object:window];
    // [notification_center removeObserver:window name:NSWindowDidResignKeyNotification object:window];
    [notification_center removeObserver:window name:NSWindowDidMiniaturizeNotification object:window];
    [notification_center removeObserver:window name:NSWindowDidDeminiaturizeNotification object:window];
    // [notification_center removeObserver:window name:NSWindowWillEnterFullScreenNotification object:window];
    // [notification_center removeObserver:window name:NSWindowDidEnterFullScreenNotification object:window];
    // [notification_center removeObserver:window name:NSWindowWillExitFullScreenNotification object:window];
    // [notification_center removeObserver:window name:@"NSWindowDidFailToEnterFullScreenNotification" object:window];
    // [notification_center removeObserver:window name:@"NSWindowDidFailToExitFullScreenNotification" object:window];

    // TODO(mtwilliams): Queue a `closed` event.
    free((void *)wrapper);
  }

  - (void)windowDidResize:(NSNotification *)notification {
    (void)notification;
    // TODO(mtwilliams): Queue a `resized` event.

    if (![self isFullscreen] && ![self isBorderless]) {
      if ([self isZoomed]) {
        // TODO(mtwilliams): Queue a `maximized` event?
      }
    }
  }

  - (void)windowDidMiniaturize:(NSNotification *)notification {
    (void)notification;
    // TODO(mtwilliams): Queue a `minimized` event.
  }

  - (void)windowDidDeminiaturize:(NSNotification *)notification {
    (void)notification;
    // TODO(mtwilliams): Queue a `restored` event.
  }

  - (void)windowDidEnterFullScreen:(NSNotification *)notification {
    (void)notification;
    // TODO(mtwilliams): Queue a `fullscreen` event.
  }

  - (void)windowDidExitFullScreen:(NSNotification *)notification {
    (void)notification;
    // TODO(mtwilliams): Queue a `windowed` event.
  }
@end

yeti_cocoa_window_t *yeti_cocoa_window_open(const yeti_cocoa_window_desc_t *desc) {
  // Center on the screen with the menu bar.
  NSScreen *screen = [[NSScreen screens] objectAtIndex:0];
  const NSRect frame = [screen frame];
  const NSRect rect = NSMakeRect((frame.size.width - desc->width) / 2,
                                 (frame.size.height - desc->height) / 2,
                                 desc->width,
                                 desc->height);

  // TODO(mtwilliams): Derive |style| from |desc|.
  const NSWindowStyleMask style = NSWindowStyleMaskTitled |
                                  NSWindowStyleMaskMiniaturizable |
                                  NSWindowStyleMaskResizable |
                                  NSWindowStyleMaskClosable;

  NSWindow *window = [[Window alloc] initWithContentRect:rect
                                     styleMask:style
                                     backing:NSBackingStoreBuffered
                                     defer:NO
                                     screen:screen];

  NSNotificationCenter *notification_center =
    [NSNotificationCenter defaultCenter];

  [notification_center addObserver:window selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:window];

  [notification_center addObserver:window selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:window];

  // TODO(mtwilliams): Use the notification center to track visibility.
  // [notification_center addObserver:window selector:@selector(windowDidExpose:) name:NSWindowDidExposeNotification object:window];
  // [notification_center addObserver:window selector:@selector(windowDidBecomeKey:) name:NSWindowDidBecomeKeyNotification object:window];
  // [notification_center addObserver:window selector:@selector(windowDidResignKey:) name:NSWindowDidResignKeyNotification object:window];
  [notification_center addObserver:window selector:@selector(windowDidMiniaturize:) name:NSWindowDidMiniaturizeNotification object:window];
  [notification_center addObserver:window selector:@selector(windowDidDeminiaturize:) name:NSWindowDidDeminiaturizeNotification object:window];

  // TODO(mtwilliams): Use the notification center to track windowed/fullscreen status.
  // [notification_center addObserver:window selector:@selector(windowWillEnterFullScreen:) name:NSWindowWillEnterFullScreenNotification object:window];
  // [notification_center addObserver:window selector:@selector(windowDidEnterFullScreen:) name:NSWindowDidEnterFullScreenNotification object:window];
  // [notification_center addObserver:window selector:@selector(windowWillExitFullScreen:) name:NSWindowWillExitFullScreenNotification object:window];
  // [notification_center addObserver:window selector:@selector(windowDidFailToEnterFullScreen:) name:@"NSWindowDidFailToEnterFullScreenNotification" object:window];
  // [notification_center addObserver:window selector:@selector(windowDidFailToExitFullScreen:) name:@"NSWindowDidFailToExitFullScreenNotification" object:window];

  [window setDelegate:window];

  // TODO(mtwilliams): Don't use `malloc`?
  yeti_cocoa_window_t *wrapper = (yeti_cocoa_window_t *)malloc(sizeof(yeti_cocoa_window_t));
  wrapper->window = window;
  window.wrapper = wrapper;
  return wrapper;
}

void yeti_cocoa_window_close(yeti_cocoa_window_t *cocoa_window) {
  // Automatically released. See `releasedWhenClosed`.
  [window->window close];
}

void yeti_cocoa_window_show(yeti_cocoa_window_t *window) {
  // TODO(mtwilliams): Implement `show`.
}

void yeti_cocoa_window_hide(yeti_cocoa_window_t *window) {
  // TODO(mtwilliams): Implement `hide`.
}

bool yeti_cocoa_window_is_minimized(const yeti_cocoa_window_t *window) {
  return !![window->window isMiniaturized];
}

void yeti_cocoa_window_minimize(yeti_cocoa_window_t *window) {
  if (!yeti_cocoa_window_is_minimized(window))
    [window->window miniaturize:nil];
}

bool yeti_cocoa_window_is_maximized(const yeti_cocoa_window_t *window) {
  return !![window->window isZoomed];
}

void yeti_cocoa_window_maximize(yeti_cocoa_window_t *window) {
  if (!yeti_cocoa_window_is_maximized(window))
    [window->window zoom:nil];
}

void yeti_cocoa_window_restore(yeti_cocoa_window_t *window) {
  if ([window->window isMiniaturized])
    [window->window deminiaturize:nil];
  if ([window->window isZoomed])
    [window->window zoom:nil];
}

// TODO(mtwilliams): Implement `clip` and `unclip`.
void yeti_cocoa_window_clip(yeti_cocoa_window_t *window) {}
void yeti_cocoa_window_unclip(yeti_cocoa_window_t *window) {}

void yeti_cocoa_window_title(const yeti_cocoa_window_t *window,
                             char title[256]) {
  if (NSString *title = [window->window title]) {
    const char *converted = [title UTF8String];
    strncpy(&title[0], converted, 256);
    title[255] = '\0';
  } else {
    title[0] = '\0';
  }
}

void yeti_cocoa_window_set_title(yeti_cocoa_window_t *window,
                                 const char *title) {
  NSString *converted = [[NSString alloc] initWithUTF8String:title];
  [window->window setTitle:converted];
  [converted release];
}

void yeti_cocoa_window_dimensions(const yeti_cocoa_window_t *window,
                                  u32 *width,
                                  u32 *height) {
  const NSRect rect = [window->window contentRectForFrameRect: [window->window frame]];
  *width = rect.size.width;
  *height = rect.size.height;
}

void yeti_cocoa_window_resize(yeti_cocoa_window_t *window,
                              u32 new_width,
                              u32 new_height) {
  if ([window->window isZoomed])
    [window->window zoom:nil];
  const NSRect frame = [window->window frame];
  const NSRect rect = NSMakeRect(frame.origin.x, frame.origin.y, new_width, new_height);
  [window->window setFrame:[window->window frameRectForContentRect:rect] display:TRUE];
}

// TODO(mtwilliams): Implement these.
void yeti_cocoa_window_set_resizeable(yeti_cocoa_window_t *window, bool resizeable) {}

void yeti_cocoa_window_set_closable(yeti_cocoa_window_t *window, bool closable) {
  window->closable = closable;
}

bool yeti_cocoa_window_focused(const yeti_cocoa_window_t *window) {
#if 1
  return !![window->window mainWindow];
#else
  return !!([NSApp mainWindow] == window->window);
#endif
}

bool yeti_cocoa_window_keyboard_focus(const yeti_cocoa_window_t *window) {
  return yeti_cocoa_window_focused(window);
}

bool yeti_cocoa_window_mouse_focus(const yeti_cocoa_window_t *window) {
  return yeti_cocoa_window_focused(window);
}

// TODO(mtwilliams): Determine if we can set keyboard and mouse focus.
 // [window->window makeMainWindow]?
// bool set_keyboard_focus(bool new_keyboard_focus);
// bool set_mouse_focus(bool new_mouse_focus);

uintptr_t yeti_cocoa_window_to_native_handle(const yeti_cocoa_window *window) {
  return (uintptr_t)window->window;
}
