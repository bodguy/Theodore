#include "os_types.h"

#ifdef Quark_MacOS

#ifndef CocoaPlatform_h
#define CocoaPlatform_h

#include <string>

@class View;
@interface View : NSOpenGLView<NSWindowDelegate> {
}
@end

namespace Quark {
    class Platform;
    class CocoaPlatform {
    public:
        CocoaPlatform();
        ~CocoaPlatform();
        
        bool CreatePlatformCocoa(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, Enumeration::WindowStyle style);
        void KillPlatformCocoa();
        
        static CocoaPlatform* instance;
        static Platform* platform;
        
        NSWindow* window;
        View* view;
    };
}

#endif /* CocoaPlatform_h */

#endif /* Quark_MacOS */
