// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Platform.h"

namespace Theodore {
  PlatformContext::PlatformContext()
      : name("No Title"), width(800), height(600), fullscreen(false), majorVersion(4), minorVersion(3), multisample(0), style(WindowStyle::Resizable), profile(ContextProfile::Core) {}

  PlatformContext::~PlatformContext() {}

  Platform* Platform::GetInstance() { return instance; }

  void Platform::Quit() { mIsRunning = false; }

  bool Platform::IsRunning() const { return mIsRunning; }

  int Platform::GetWidth() { return GetInstance()->mWidth; }

  int Platform::GetHeight() { return GetInstance()->mHeight; }

  void Platform::LogSystemInfo() {
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

  bool Platform::QueryExtentionSupported(const std::string& extionsion_name) {
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
}  // namespace Theodore