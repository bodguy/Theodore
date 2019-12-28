// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "os_types.h"

#ifdef Theodore_Windows

#ifndef WindowsPlatform_h
#define WindowsPlatform_h

#include <string>

#include "Graphics/Enumeration.h"

namespace Theodore {
  class WindowsPlatform {
    friend class Platform;

  private:
    WindowsPlatform();
    ~WindowsPlatform();

    bool CreatePlatformWindows(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, WindowStyle style, ContextProfile profile);
    void KillPlatformWindows();

    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    static bool QueryWGLExtensionSupported(const std::string& extionsion_name);

    static WindowsPlatform* instance;
    static Platform* platform;

    HINSTANCE mhInstance;
    HWND mHandle;
    HDC mHdc;
    HGLRC mContext;
  };
}  // namespace Theodore

#endif /* WindowsPlatform_h */

#endif /* Theodore_Windows */
