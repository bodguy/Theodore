// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "CondiitonalVars.h"

#ifdef Theodore_Linux

#ifndef LinuxPlatform_h
#define LinuxPlatform_h

#include <string>

namespace Theodore {
  class LinuxPlatform {
    friend class Platform;

  private:
    LinuxPlatform();
    ~LinuxPlatform();

    bool CreatePlatformLinux(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, WindowStyle style, ContextProfile profile);
    void KillPlatformLinux();

    static LinuxPlatform* instance;
    static Platform* platform;

    Display* mDisplay;
    Window mWindow;
    int mScreen;
    GLXContext mContext;
    Atom mDestroyMessage;
  };
}  // namespace Theodore

#endif /* LinuxPlatform_h */

#endif /* Theodore_Linux */
