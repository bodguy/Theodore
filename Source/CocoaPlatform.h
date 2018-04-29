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
    class CocoaPlatform {
		friend class Platform;
    private:
        CocoaPlatform();
        ~CocoaPlatform();
        
        bool CreatePlatformCocoa(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, Enumeration::WindowStyle style);
        void KillPlatformCocoa();
        
        static CocoaPlatform* instance;
        static Platform* platform;
        
        NSWindow* window;
        View* view;

    public:
        static CocoaPlatform* GetInstance();
    };
}

#endif /* CocoaPlatform_h */

#endif /* Quark_MacOS */
