// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/window.h>
#include <bt/win32/resources.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace bt {
  static LRESULT WINAPI window_proc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam )
  {
    Window* window = (Window*)GetPropA(hWnd, "bt::Window");
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }

  namespace Windows {
    void initialize()
    {
      WNDCLASSEXW wcx;
      zero((void*)&wcx, sizeof(WNDCLASSEX));
      wcx.cbSize        = sizeof(WNDCLASSEX);
      wcx.style         = CS_VREDRAW | CS_HREDRAW;
      wcx.lpfnWndProc   = &window_proc;
      wcx.hInstance     = GetModuleHandle(NULL);
      wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
      wcx.hIcon         = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_ENGINE_ICON));
      wcx.hIconSm       = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_ENGINE_ICON));
      wcx.lpszClassName = L"Butane";

      if (!RegisterClassExW(&wcx))
        fail("Unable to register WndClassEx `Butane`!\n");

      // Register for keyboard and mouse raw-input events:
      // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
      RAWINPUTDEVICE rids[2];

      // Keyboard:
      rids[0].usUsagePage = 0x01;
      rids[0].usUsage = 0x06;
      rids[0].hwndTarget = NULL;
      rids[0].dwFlags = 0;

      // Mouse:
      rids[1].usUsagePage = 0x01;
      rids[1].usUsage = 0x02;
      rids[1].hwndTarget = NULL;
      rids[1].dwFlags = 0;

      if (!RegisterRawInputDevices(&rids[0], 2, sizeof(RAWINPUTDEVICE)))
        fail("Unable to register RawInput devices!\n");
    }

    void update()
    {
      MSG msg;
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  } // Windows
} // bt