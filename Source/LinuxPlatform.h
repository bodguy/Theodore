/**
  @brief linux platform dependent class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#include "os_types.h"

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

    bool CreatePlatformLinux(const std::string& title, int width, int height, bool fullscreen,
                             int majorVersion, int minorVersion, int multisample, WindowStyle style,
                             ContextProfile profile);
    void KillPlatformLinux();

    static LinuxPlatform* instance;
    static Platform* platform;

    Display* mDisplay;
    Window mWindow;
    int mScreen;
    GLXContext mContext;
    Atom mDestroyMessage;
  };
}

#endif /* LinuxPlatform_h */

#endif /* Theodore_Linux */
