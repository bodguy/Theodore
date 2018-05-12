#include "os_types.h"

#ifdef Quark_Linux

#ifndef LinuxPlatform_h
#define LinuxPlatform_h

#include <string>

namespace Quark {
    class LinuxPlatform {
		friend class Platform;
	private:
		LinuxPlatform();
		~LinuxPlatform();

		bool CreatePlatformLinux(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, WindowStyle style);
		void KillPlatformLinux();

        static LinuxPlatform* instance;
        static Platform *platform;

        Display* mDisplay;
        Window mWindow;
        int mScreen;
        GLXContext mContext;
        Atom mDestroyMessage;
    };
}

#endif /* LinuxPlatform_h */

#endif /* Quark_Linux */
