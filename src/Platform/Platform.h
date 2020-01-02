// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Platform_h
#define Platform_h

#include <string>

#include "Graphics/Enumeration.h"
#include "Helper/Debug.h"
#include "KeyCode.h"
#include "Math/Math.h"
#include "Math/Vector3d.h"

namespace Theodore {
  typedef Vector3d PointCoord;
  class PlatformContext {
  public:
    PlatformContext();
    ~PlatformContext();

    std::string name;
    int width;
    int height;
    bool fullscreen;
    int majorVersion;
    int minorVersion;
    int multisample;
    WindowStyle style;
    ContextProfile profile;
  };

  class Platform {
    friend class Input;
    friend class WindowsPlatform;
    friend class CocoaPlatform;
    friend class LinuxPlatform;

  public:
    Platform();
    ~Platform();

    static Platform* GetInstance();
    bool Initialize(const PlatformContext& param);
    void Update();
    void SwapBuffer();
    void WindowSizeChanged(int width, int height);
    const PointCoord CenterOnWindow();
    void GetDesktopSize(int& width, int& height);
    void SetMousePos(const int x, const int y) const;
    int GetScreenDPI();
    void SetVSync(bool sync);
    int GetVSync();
    bool IsFocus() const;

    void Quit();
    bool IsRunning() const;
    static int GetWidth();
    static int GetHeight();
    static void ChangeTitle(const std::string& titleName);
    static void LogSystemInfo();
    static bool QueryExtentionSupported(const std::string& extionsion_name);

  public:
    static Platform* instance;

    int mWidth, mHeight;
    std::string mTitle;
    bool mIsShowCursor;
    bool mIsFullScreen;
    bool mKeys[KEY_MAX];
    mutable bool mIsFocused;
    unsigned short mLocalKeymap[KEY_MAX];
    bool mMouseButtons[MOUSE_BUTTON_MAX];
    bool mIsRunning;
    Vector3d mMousePosition;
    bool mIsMultisampleSupported;
  };
}  // namespace Theodore

#endif /* Platform_h */
