/**
	@brief platform independent interface
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef Platform_h
#define Platform_h

#include <string>
#include "Vector3d.h"
#include "KeyCode.h"
#include "Enumeration.h"

namespace Theodore {
    typedef Vector3d PointCoord;
	class PlatformContext {
	public:
		PlatformContext() :name("No Title"), width(800), height(600), fullscreen(false), majorVersion(4), minorVersion(3), multisample(0), style(WindowStyle::Resizable), profile(ContextProfile::Core) {}
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
        
	private:
        static Platform* instance;
        
        int mWidth, mHeight;
        std::string mTitle;
        bool mIsShowCursor;
        bool mIsFullScreen;
        bool mKeys[KEY_MAX];
        int mLocalKeymap[KEY_MAX];
        bool mMouseButtons[MOUSE_BUTTON_MAX];
		bool mIsRunning;
        Vector3d mMousePosition;
		bool mIsMultisampleSupported;
	};
}

#endif /* Platform_h */
