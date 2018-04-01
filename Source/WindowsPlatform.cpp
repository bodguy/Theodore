#include "os_types.h"

#ifdef Quark_Windows

// the including order is matters!
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include GLEW_INCLUDE_DIR
#include GLEW_INCLUDE_DIR2
#include "Platform.h"
#include "WindowsPlatform.h"
#include "Debug.h"

namespace Quark {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WindowsPlatform definition

	WindowsPlatform* WindowsPlatform::instance = NULL;
	Platform* WindowsPlatform::platform = NULL;
	WindowsPlatform::WindowsPlatform() : mhInstance(NULL), mHandle(NULL), mHdc(NULL), mContext(NULL) {

	}

	WindowsPlatform::~WindowsPlatform() {

	}

	bool WindowsPlatform::CreatePlatformWindows(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, Enumeration::WindowStyle style) {
		mhInstance = GetModuleHandle(NULL);
		if (!mhInstance) return false;

		DWORD mStyle;
		DWORD mExStyle;
		platform->mIsFullScreen = fullscreen;

		if (platform->mIsFullScreen) {
			DEVMODE dmScreenSettings;
			if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings))
				return false;

			platform->GetDesktopSize(platform->mWidth, platform->mHeight);
			platform->mIsShowCursor = false;

			// NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				return false;

			mExStyle = WS_EX_APPWINDOW;
			mStyle = WS_POPUP | WS_EX_TOPMOST;
		} else {
			// setting windowed mode
			mExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			if (style == Enumeration::Resizable) {
				mStyle = WS_OVERLAPPEDWINDOW;
			} else if (style == Enumeration::NonResizeable) {
				mStyle = WS_CAPTION | WS_SYSMENU;
			}
			platform->mWidth = width;
			platform->mHeight = height;
		}
		platform->mTitle = title;

		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			WndProc,
			0L,
			0L,
			mhInstance,
			LoadIcon(mhInstance,NULL), // icon
			LoadCursor(mhInstance,NULL), // cursor
			NULL,
			NULL,
			platform->mTitle.c_str(),
			NULL
		};
		if (!RegisterClassEx(&wc)) return false;

		RECT rt = { 0, 0, platform->mWidth, platform->mHeight };
		if (!AdjustWindowRectEx(&rt, mStyle, false, mExStyle))
			return false;

		DWORD w = rt.right - rt.left;
		DWORD h = rt.bottom - rt.top;

		const PointCoord p = platform->CenterOnWindow();

		/*
			https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
			CreateWindow -> GetDC -> ChoosePixelFormat -> SetPixelFormat -> wglCreateContext -> wglMakeCurrent -> wglGetProcAddress
									 wglChoosePixelFormatARB				wglCreateContextAttribsARB
		*/
		mHandle = CreateWindowEx(mExStyle, platform->mTitle.c_str(), platform->mTitle.c_str(),
			mStyle, static_cast<int>(p.x), static_cast<int>(p.y), w, h, NULL, NULL, mhInstance, NULL);
		if (!mHandle) return false;

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		mHdc = GetDC(mHandle);
		if (!mHdc) return false;
		int nPixelFormat = ChoosePixelFormat(mHdc, &pfd);
		if (!nPixelFormat) return false;
		if (!SetPixelFormat(mHdc, nPixelFormat, &pfd)) return false;
		glewExperimental = GL_TRUE;

		mContext = wglCreateContext(mHdc);
		if (!mContext) return false;
		if (!wglMakeCurrent(mHdc, mContext)) return false;

		// We need to query about modern opengl functions.
		if (majorVersion != 0 || minorVersion != 0) {
			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
			wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
			if (wglChoosePixelFormatARB == nullptr) {
				Debug::Log("wglChoosePixelFormatARB() failed.\n");
				return false;
			}

			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
			wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
			if (wglCreateContextAttribsARB == nullptr) {
				Debug::Log("wglCreateContextAttribsARB() failed.\n");
				return false;
			}

			// To create, real window
			HWND handle = CreateWindowEx(mExStyle, platform->mTitle.c_str(), platform->mTitle.c_str(),
				mStyle, static_cast<int>(p.x), static_cast<int>(p.y), w, h, NULL, NULL, mhInstance, NULL);
			if (!handle) return false;

			HDC hdc = GetDC(handle);
			if (!hdc) return false;

			const int pixelAttribs[] = {
				WGL_SAMPLES_ARB, 16,
				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_RED_BITS_ARB, 8,
				WGL_GREEN_BITS_ARB, 8,
				WGL_BLUE_BITS_ARB, 8,
				WGL_ALPHA_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				0
			};

			int pixelFormatID; UINT numFormats;
			int nPixelFormat2 = wglChoosePixelFormatARB(hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
			if (!nPixelFormat2 || numFormats == 0) return false;

			PIXELFORMATDESCRIPTOR pfd2;
			DescribePixelFormat(hdc, pixelFormatID, sizeof(pfd2), &pfd2);
			SetPixelFormat(hdc, pixelFormatID, &pfd2);

			const int major_min = majorVersion, minor_min = minorVersion;
			int  contextAttribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				//		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
				0
			};

			HGLRC context = wglCreateContextAttribsARB(hdc, 0, contextAttribs);
			if (!context) return false;

			// destory old context
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(mContext);
			ReleaseDC(mHandle, mHdc);
			DestroyWindow(mHandle);

			if (!wglMakeCurrent(hdc, context)) return false;
			mHandle = handle;
			mHdc = hdc;
			mContext = context;
		}
		// now we can init glew lib
		if (glewInit() != GLEW_OK) return false;

		Debug::Log("Renderer: %s\n", glGetString(GL_RENDERER));
		Debug::Log("OpenGL version supported by this platform (%s) \n", glGetString(GL_VERSION));
		Debug::Log("GLSL version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ShowWindow(mHandle, SW_SHOWDEFAULT);
		UpdateWindow(mHandle);
		ShowCursor(platform->mIsShowCursor);
		SetForegroundWindow(mHandle);
		SetFocus(mHandle);

		return true;
	}

	void WindowsPlatform::KillPlatformWindows() {
		if (platform->mIsFullScreen) {
			ChangeDisplaySettings(NULL, 0); // back to the desktop
			ShowCursor(true);
		}
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(mContext);
		ReleaseDC(mHandle, mHdc);
		DestroyWindow(mHandle);
		UnregisterClass(platform->mTitle.c_str(), mhInstance);
	}

	LRESULT CALLBACK WindowsPlatform::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		//case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
			// zDelta : Indicates that the mouse wheel was pressed, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
			instance->platform->mMousePosition.z = (float)(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		case WM_SIZE:
			instance->platform->WindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
			break;
		// WM_CLOSE -> WM_DESTROY -> WM_QUIT
		//case WM_CLOSE:
		//	DestroyWindow(hWnd);
		//	break;
		//case WM_DESTROY:
		//	PostQuitMessage(0);
		//	break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	bool WindowsPlatform::QueryWGLExtensionSupported(const std::string& extionsion_name)
	{
		// this is pointer to function which returns pointer to string with list of all wgl extensions
		PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

		// determine pointer to wglGetExtensionsStringEXT function
		_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

		if (strstr(_wglGetExtensionsStringEXT(), extionsion_name.c_str()) == NULL)
		{
			// string was not found
			return false;
		}

		// extension is supported
		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Platform definition

	Platform* Platform::instance = NULL;
	Platform::Platform() :mWidth(0), mHeight(0), mIsShowCursor(true), mIsRunning(true), mMousePosition() {
		WindowsPlatform::instance = new WindowsPlatform();
		instance = this;
		WindowsPlatform::instance->platform = this;

		for (int i = 0; i < KEY_MAX; i++) {
			mKeys[i] = false;
			mLocalKeymap[i] = KEY_UNDEFINED;
		}

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
			mMouseButtons[i] = false;

		{
			mLocalKeymap[KEY_A] = 'A';
			mLocalKeymap[KEY_B] = 'B';
			mLocalKeymap[KEY_C] = 'C';
			mLocalKeymap[KEY_D] = 'D';
			mLocalKeymap[KEY_E] = 'E';
			mLocalKeymap[KEY_F] = 'F';
			mLocalKeymap[KEY_G] = 'G';
			mLocalKeymap[KEY_H] = 'H';
			mLocalKeymap[KEY_I] = 'I';
			mLocalKeymap[KEY_J] = 'J';
			mLocalKeymap[KEY_K] = 'K';
			mLocalKeymap[KEY_L] = 'L';
			mLocalKeymap[KEY_M] = 'M';
			mLocalKeymap[KEY_N] = 'N';
			mLocalKeymap[KEY_O] = 'O';
			mLocalKeymap[KEY_P] = 'P';
			mLocalKeymap[KEY_Q] = 'Q';
			mLocalKeymap[KEY_R] = 'R';
			mLocalKeymap[KEY_S] = 'S';
			mLocalKeymap[KEY_T] = 'T';
			mLocalKeymap[KEY_U] = 'U';
			mLocalKeymap[KEY_V] = 'V';
			mLocalKeymap[KEY_W] = 'W';
			mLocalKeymap[KEY_X] = 'X';
			mLocalKeymap[KEY_Y] = 'Y';
			mLocalKeymap[KEY_Z] = 'Z';

			mLocalKeymap[KEY_0] = '0';
			mLocalKeymap[KEY_1] = '1';
			mLocalKeymap[KEY_2] = '2';
			mLocalKeymap[KEY_3] = '3';
			mLocalKeymap[KEY_4] = '4';
			mLocalKeymap[KEY_5] = '5';
			mLocalKeymap[KEY_6] = '6';
			mLocalKeymap[KEY_7] = '7';
			mLocalKeymap[KEY_8] = '8';
			mLocalKeymap[KEY_9] = '9';

			mLocalKeymap[KEY_SPACE] = 0x20;
			mLocalKeymap[KEY_SEMICOLON] = 0xBA;
			mLocalKeymap[KEY_EQUALS] = 0xBB;
			mLocalKeymap[KEY_APOSTROPHE] = 0xDE;
			mLocalKeymap[KEY_COMMA] = 0xBC;
			mLocalKeymap[KEY_MINUS] = 0xBD;
			mLocalKeymap[KEY_PERIOD] = 0xBE;
			mLocalKeymap[KEY_SLASH] = 0xBF;
			mLocalKeymap[KEY_LEFTBRACKET] = 0xDB;
			mLocalKeymap[KEY_BACKSLASH] = 0xDC;
			mLocalKeymap[KEY_RIGHTBRACKET] = 0xDD;
			mLocalKeymap[KEY_GRAVE_ACCENT] = 0xC0;

			mLocalKeymap[KEY_ESCAPE] = VK_ESCAPE;
			mLocalKeymap[KEY_RETURN] = VK_RETURN;
			mLocalKeymap[KEY_TAB] = VK_TAB;
			mLocalKeymap[KEY_BACKSPACE] = VK_BACK;
			mLocalKeymap[KEY_INSERT] = VK_INSERT;
			mLocalKeymap[KEY_DELETE] = VK_DELETE;
			mLocalKeymap[KEY_UP] = VK_UP;
			mLocalKeymap[KEY_DOWN] = VK_DOWN;
			mLocalKeymap[KEY_RIGHT] = VK_RIGHT;
			mLocalKeymap[KEY_LEFT] = VK_LEFT;
			mLocalKeymap[KEY_PAGEUP] = VK_PRIOR;
			mLocalKeymap[KEY_PAGEDOWN] = VK_NEXT;
			mLocalKeymap[KEY_HOME] = VK_HOME;
			mLocalKeymap[KEY_END] = VK_END;
			mLocalKeymap[KEY_CAPSLOCK] = VK_CAPITAL;
			mLocalKeymap[KEY_SCROLLOCK] = VK_SCROLL;
			mLocalKeymap[KEY_NUMLOCK] = VK_NUMLOCK;
			mLocalKeymap[KEY_PRINTSCREEN] = VK_SNAPSHOT;
			mLocalKeymap[KEY_PAUSE] = VK_PAUSE;

			mLocalKeymap[KEY_F1] = VK_F1;
			mLocalKeymap[KEY_F2] = VK_F2;
			mLocalKeymap[KEY_F3] = VK_F3;
			mLocalKeymap[KEY_F4] = VK_F4;
			mLocalKeymap[KEY_F5] = VK_F5;
			mLocalKeymap[KEY_F6] = VK_F6;
			mLocalKeymap[KEY_F7] = VK_F7;
			mLocalKeymap[KEY_F8] = VK_F8;
			mLocalKeymap[KEY_F9] = VK_F9;
			mLocalKeymap[KEY_F10] = VK_F10;
			mLocalKeymap[KEY_F11] = VK_F11;
			mLocalKeymap[KEY_F12] = VK_F12;

			mLocalKeymap[KEY_LSHIFT] = VK_LSHIFT;
			mLocalKeymap[KEY_LCTRL] = VK_LCONTROL;
			mLocalKeymap[KEY_LALT] = VK_LMENU;
			mLocalKeymap[KEY_LSUPER] = VK_LWIN;
			mLocalKeymap[KEY_RSHIFT] = VK_RSHIFT;
			mLocalKeymap[KEY_RCTRL] = VK_RCONTROL;
			mLocalKeymap[KEY_RALT] = VK_RMENU;
			mLocalKeymap[KEY_RSUPER] = VK_RWIN;
			mLocalKeymap[KEY_MENU] = VK_APPS;
			mLocalKeymap[KEY_HANGUL] = VK_HANGUL;
			mLocalKeymap[KEY_HANJA] = VK_HANJA;

			mLocalKeymap[KEY_KP_0] = VK_NUMPAD0;
			mLocalKeymap[KEY_KP_1] = VK_NUMPAD1;
			mLocalKeymap[KEY_KP_2] = VK_NUMPAD2;
			mLocalKeymap[KEY_KP_3] = VK_NUMPAD3;
			mLocalKeymap[KEY_KP_4] = VK_NUMPAD4;
			mLocalKeymap[KEY_KP_5] = VK_NUMPAD5;
			mLocalKeymap[KEY_KP_6] = VK_NUMPAD6;
			mLocalKeymap[KEY_KP_7] = VK_NUMPAD7;
			mLocalKeymap[KEY_KP_8] = VK_NUMPAD8;
			mLocalKeymap[KEY_KP_9] = VK_NUMPAD9;

			mLocalKeymap[KEY_KP_DECIMAL] = VK_DECIMAL;
			mLocalKeymap[KEY_KP_DIVIDE] = VK_DIVIDE;
			mLocalKeymap[KEY_KP_MULTIPLY] = VK_MULTIPLY;
			mLocalKeymap[KEY_KP_SUBTRACT] = VK_SUBTRACT;
			mLocalKeymap[KEY_KP_ADD] = VK_ADD;
			mLocalKeymap[KEY_KP_ENTER] = KEY_UNDEFINED; // VK_RETURN with lParam & 0x1000000;
			mLocalKeymap[KEY_KP_EQUAL] = KEY_UNDEFINED;
		}
	}

	Platform::~Platform() {
		WindowsPlatform::instance->KillPlatformWindows();
		if (WindowsPlatform::instance) {
			delete WindowsPlatform::instance;
			WindowsPlatform::instance = NULL;
		}
	}

	bool Platform::Initialize(const std::string& name, int width, int height, bool fullscreen, int majorVersion, int minorVersion, Enumeration::WindowStyle style) {
		return WindowsPlatform::instance->CreatePlatformWindows(name, width, height, fullscreen, majorVersion, minorVersion, style);
	}

	void Platform::Update() {
		MSG msg = { 0, };
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				mIsRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		mMouseButtons[MOUSE_LEFT] = static_cast<bool>(GetAsyncKeyState(VK_LBUTTON));
		mMouseButtons[MOUSE_RIGHT] = static_cast<bool>(GetAsyncKeyState(VK_RBUTTON));
		mMouseButtons[MOUSE_MIDDLE] = static_cast<bool>(GetAsyncKeyState(VK_MBUTTON));

		for (int i = 0; i < KEY_MAX; i++) {
			mKeys[i] = static_cast<bool>(GetAsyncKeyState(mLocalKeymap[i]));
		}

		POINT mousePoint;
		GetCursorPos(&mousePoint);
		ScreenToClient(WindowsPlatform::instance->mHandle, &mousePoint);
		mMousePosition.x = (float)mousePoint.x;
		mMousePosition.y = (float)mousePoint.y;
	}

	void Platform::SwapBuffer() {
		SwapBuffers(WindowsPlatform::instance->mHdc);
	}

	void Platform::WindowSizeChanged(int width, int height) {
		mWidth = width;
		mHeight = height;
		glViewport(0, 0, mWidth, mHeight);
	}

	const PointCoord Platform::CenterOnWindow() {
		int w, h;
		GetDesktopSize(w, h);

		return PointCoord((w - mWidth) / 2.f, (h - mHeight) / 2.f);
	}

	void Platform::GetDesktopSize(int& width, int& height) {
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}

	void Platform::SetMousePos(const int x, const int y) const {
		POINT mousePoint;
		mousePoint.x = static_cast<LONG>(x);
		mousePoint.y = static_cast<LONG>(y);
		ClientToScreen(WindowsPlatform::instance->mHandle, &mousePoint);
		SetCursorPos(mousePoint.x, mousePoint.y);
	}

	int Platform::GetScreenDPI() {
		HDC screen = GetDC(NULL);
		int iDPI = -1; // assume failure
		if (screen) {
			iDPI = GetDeviceCaps(screen, LOGPIXELSX);
			ReleaseDC(NULL, screen);
		}
		return iDPI;
	}

	void Platform::SetVSync(bool sync) {
		// 1 is enable, 0 is disable
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		
		if (WindowsPlatform::QueryWGLExtensionSupported("WGL_EXT_swap_control")) {
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			if(wglSwapIntervalEXT) wglSwapIntervalEXT(sync);
		}
	}

	int Platform::GetVSync() {
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;

		if (WindowsPlatform::QueryWGLExtensionSupported("WGL_EXT_swap_control")) {
			wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
			if (wglGetSwapIntervalEXT) {
				return wglGetSwapIntervalEXT();
			}
		}

		return 0;
	}
}

#endif
