#ifndef Platform_h
#define Platform_h

#include <string>
#include "Vector3d.h"
#include "KeyCode.h"
#include "Enumeration.h"

namespace Quark {
    typedef Vector3d PointCoord;
    class Platform {
        friend class Input;
        friend class WindowsPlatform;
        friend class CocoaPlatform;
        friend class LinuxPlatform;
    public:
        Platform();
        ~Platform();
        
        static Platform* GetInstance() { return instance; }
        bool Initialize(const std::string& title, int width, int height,
                        bool fullscreen = false, int majorVersion = 0, int minorVersion = 0, Enumeration::WindowStyle style = Enumeration::Resizable);
        void Update();
        void SwapBuffer();
        void WindowSizeChanged(int width, int height);
        const PointCoord CenterOnWindow();
        void GetDesktopSize(int& width, int& height);
        void SetMousePos(const int x, const int y) const;
		int GetScreenDPI();
		void SetVSync(bool sync);
		int GetVSync();

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
    };
}

#endif /* Platform_h */
