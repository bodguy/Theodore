/**
  @brief platform dependent class only for macos support
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#include "os_types.h"

#ifdef Theodore_MacOS

#ifndef CocoaPlatform_h
#define CocoaPlatform_h

#include <string>
#include "../Graphics/Enumeration.h"

@class View;
@interface View : NSOpenGLView <NSWindowDelegate> {
}
@end

namespace Theodore {
  class CocoaPlatform {
    friend class Platform;

  private:
    CocoaPlatform();
    ~CocoaPlatform();

    bool CreatePlatformCocoa(const std::string& title, int width, int height, bool fullscreen,
                             int majorVersion, int minorVersion, int multisample, WindowStyle style,
                             ContextProfile profile);

    static CocoaPlatform* instance;
    static Platform* platform;

    NSWindow* window;
    View* view;

  public:
    static CocoaPlatform* GetInstance();
    
    void KillPlatformCocoa();
  };
}

#endif /* CocoaPlatform_h */

#endif /* Theodore_MacOS */
