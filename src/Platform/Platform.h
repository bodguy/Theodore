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
    PlatformContext() : name("No Title"), width(800), height(600), fullscreen(false), majorVersion(4), minorVersion(3), multisample(0), style(WindowStyle::Resizable), profile(ContextProfile::Core) {}
    ~PlatformContext() {}

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

    static Platform* GetInstance() { return instance; }
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

    void Quit() { mIsRunning = false; }
    bool IsRunning() const { return mIsRunning; }
    static int GetWidth() { return GetInstance()->mWidth; }
    static int GetHeight() { return GetInstance()->mHeight; }
    static void ChangeTitle(const std::string& titleName);
    static void LogSystemInfo() {
      Debug::Log("Vendor              : %s", glGetString(GL_VENDOR));
      Debug::Log("Renderer            : %s", glGetString(GL_RENDERER));
      Debug::Log("Version             : %s", glGetString(GL_VERSION));
      Debug::Log("GLSL                : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

      int param;
      glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &param);
      if (param == GL_CONTEXT_CORE_PROFILE_BIT) {
        Debug::Log("Context Profile   : Core");
      } else {
        Debug::Log("Context Profile   : Compatibility");
      }
      // if (QueryExtentionSupported("GL_ARB_get_program_binary")) {
      //	Debug::Log("program binary supported!\n");
      //}
    }

    static bool QueryExtentionSupported(const std::string& extionsion_name) {
      GLint n;
      glGetIntegerv(GL_NUM_EXTENSIONS, &n);
      for (int i = 0; i < n; i++) {
        std::string ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
        Debug::Log(ext);
        if (ext == extionsion_name) {
          return true;
        }
      }

      return false;
    }

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
