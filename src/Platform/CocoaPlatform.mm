#include "os_types.h"

#ifdef Theodore_MacOS

// the including order is matters!
#include GLEW_INCLUDE_DIR
#import <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "Platform.h"
#include "CocoaPlatform.h"
#include "../Helper/Utility.h"
#include "KeyCode.h"
#include "../Helper/Debug.h"

namespace Theodore {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CocoaPlatform definition
	// TODO: mIsMultisampleSupported

	CocoaPlatform* CocoaPlatform::instance = NULL;
	Platform* CocoaPlatform::platform = NULL;
	CocoaPlatform::CocoaPlatform() :window(NULL), view(NULL) {}

	CocoaPlatform::~CocoaPlatform() {}

	bool CocoaPlatform::CreatePlatformCocoa(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, WindowStyle style, ContextProfile profile) {
		@autoreleasepool {
      platform->mTitle = title;
			platform->mWidth = width;
			platform->mHeight = height;
      platform->mIsFullScreen = fullscreen;

			[NSApplication sharedApplication];
      NSUInteger windowStyle = 0;
      NSRect windowRect;

			if (platform->mIsFullScreen) {
        windowRect = [[NSScreen mainScreen] frame];
        windowStyle = NSWindowStyleMaskBorderless;
        window = [[CocoaWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
        [window setLevel:NSMainMenuWindowLevel+1];
        platform->mIsShowCursor = false;
			} else {
        const PointCoord pos = platform->CenterOnWindow();
        windowRect = NSMakeRect(pos.x, pos.y, platform->mWidth, platform->mHeight); // x, y, w, h

        windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
        if (style == WindowStyle::Resizable) {
          windowStyle |= NSWindowStyleMaskResizable;
        }
        window = [[CocoaWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
        [window setLevel:NSNormalWindowLevel];
			}

      NSWindowController* windowController = [[NSWindowController alloc] initWithWindow: window];
      [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];

      if (!PrepareContext(windowRect, majorVersion, minorVersion, multisample, profile))
        return false;

      Platform::LogSystemInfo();
			[window setAcceptsMouseMovedEvents:YES];
			[window setContentView:view];
			[window setDelegate:view];

			[window setTitle: CocoaPlatform::toNSString(platform->mTitle)];
      [window makeKeyAndOrderFront:window];
			[window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
			[window orderFrontRegardless];

			if (platform->mIsShowCursor) {
        [NSCursor unhide];
			} else {
        [NSCursor hide];
			}

			return true;
		}
	}

  bool CocoaPlatform::PrepareContext(const NSRect windowRect, int majorVersion, int minorVersion, int multisample, const ContextProfile profile) {
    // major, minor version deprecated
	  const unsigned int openGLVersion = profile == ContextProfile::Core ? NSOpenGLProfileVersion4_1Core : NSOpenGLProfileVersionLegacy;

    NSOpenGLPixelFormatAttribute windowedAttrs[] =
    {
      NSOpenGLPFAMultisample,
      NSOpenGLPFASampleBuffers, static_cast<NSOpenGLPixelFormatAttribute>(multisample ? 1 : 0),
      NSOpenGLPFASamples, static_cast<NSOpenGLPixelFormatAttribute>(multisample),
      NSOpenGLPFAAccelerated,
      NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAColorSize, 24,
      NSOpenGLPFADepthSize, 24,
      NSOpenGLPFAAlphaSize, 8,
      NSOpenGLPFAStencilSize, 8,
      NSOpenGLPFAOpenGLProfile, openGLVersion,
      0
    };

    // try to choose a supported pixel format
    NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:windowedAttrs];
    if(!pf) return false;

    view = [[View alloc] initWithFrame:windowRect pixelFormat:pf];
    if (!view) return false;
    [[view openGLContext] makeCurrentContext];

    GLint swapInt = 1;
    [[view openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) return false;

    return true;
	}

	void CocoaPlatform::KillPlatformCocoa() {
		[view clearGLContext];
		[NSApp terminate:window];
	}

	CocoaPlatform* CocoaPlatform::GetInstance() {
		return CocoaPlatform::instance;
	}

	NSString* CocoaPlatform::toNSString(const std::string& str) {
    return [NSString stringWithCString:str.c_str() encoding:[NSString defaultCStringEncoding]];
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Platform definition

	Platform* Platform::instance = NULL;
	Platform::Platform() :mWidth(0), mHeight(0), mIsShowCursor(true), mIsRunning(true), mMousePosition() {
		CocoaPlatform::instance = new CocoaPlatform();
		instance = this;
		CocoaPlatform::instance->platform = this;

		for (int i = 0; i < KEY_MAX; i++) {
			mKeys[i] = false;
			mLocalKeymap[i] = KEY_UNDEFINED;
		}

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
			mMouseButtons[i] = false;

		{ // optimized to Apple Extended Keyboard II (domestic layout)
			mLocalKeymap[0] = KEY_A;
      mLocalKeymap[1] = KEY_S;
      mLocalKeymap[2] = KEY_D;
      mLocalKeymap[3] = KEY_F;
      mLocalKeymap[4] = KEY_H;
      mLocalKeymap[5] = KEY_G;
      mLocalKeymap[6] = KEY_Z;
      mLocalKeymap[7] = KEY_X;
      mLocalKeymap[8] = KEY_C;
      mLocalKeymap[9] = KEY_V;
      mLocalKeymap[10] = KEY_UNDEFINED; // for historical reason
			mLocalKeymap[11] = KEY_B;
      mLocalKeymap[12] = KEY_Q;
      mLocalKeymap[13] = KEY_W;
			mLocalKeymap[14] = KEY_E;
      mLocalKeymap[15] = KEY_R;
      mLocalKeymap[16] = KEY_Y;
      mLocalKeymap[17] = KEY_T;
      mLocalKeymap[18] = KEY_1;
      mLocalKeymap[19] = KEY_2;
      mLocalKeymap[20] = KEY_3;
      mLocalKeymap[21] = KEY_4;
      mLocalKeymap[22] = KEY_6;
      mLocalKeymap[23] = KEY_5;
      mLocalKeymap[24] = KEY_EQUALS;
      mLocalKeymap[25] = KEY_9;
      mLocalKeymap[26] = KEY_7;
      mLocalKeymap[27] = KEY_MINUS;
      mLocalKeymap[28] = KEY_8;
      mLocalKeymap[29] = KEY_0;
      mLocalKeymap[30] = KEY_RIGHTBRACKET;
			mLocalKeymap[31] = KEY_O;
      mLocalKeymap[32] = KEY_U;
      mLocalKeymap[33] = KEY_LEFTBRACKET;
      mLocalKeymap[34] = KEY_I;
			mLocalKeymap[35] = KEY_P;
      mLocalKeymap[36] = KEY_RETURN;
      mLocalKeymap[37] = KEY_L;
      mLocalKeymap[38] = KEY_J;
      mLocalKeymap[39] = KEY_APOSTROPHE;
      mLocalKeymap[40] = KEY_K;
      mLocalKeymap[41] = KEY_SEMICOLON;
      mLocalKeymap[42] = KEY_BACKSLASH;
      mLocalKeymap[43] = KEY_COMMA;
      mLocalKeymap[44] = KEY_SLASH;
      mLocalKeymap[45] = KEY_N;
      mLocalKeymap[46] = KEY_M;
      mLocalKeymap[47] = KEY_PERIOD;
      mLocalKeymap[48] = KEY_TAB;
      mLocalKeymap[49] = KEY_SPACE;
      mLocalKeymap[50] = KEY_GRAVE_ACCENT;
      mLocalKeymap[51] = KEY_BACKSPACE;
      mLocalKeymap[52] = KEY_KP_ENTER;
      mLocalKeymap[53] = KEY_ESCAPE;
      mLocalKeymap[54] = KEY_RSUPER;
      mLocalKeymap[55] = KEY_LSUPER;
      mLocalKeymap[56] = KEY_LSHIFT;
      mLocalKeymap[57] = KEY_CAPSLOCK;
      mLocalKeymap[58] = KEY_LALT;
      mLocalKeymap[59] = KEY_LCTRL;
      mLocalKeymap[60] = KEY_RSHIFT;
      mLocalKeymap[61] = KEY_RALT;
      mLocalKeymap[62] = KEY_RCTRL;
      mLocalKeymap[63] = KEY_UNDEFINED; // KEY_RGUI
      mLocalKeymap[64] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[65] = KEY_UNDEFINED; // KEY_KP_PERIOD
      mLocalKeymap[66] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[67] = KEY_KP_MULTIPLY;
      mLocalKeymap[68] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[69] = KEY_KP_ADD;
      mLocalKeymap[70] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[71] = KEY_UNDEFINED; // KEY_NUMLOCKCLEAR
      mLocalKeymap[72] = KEY_UNDEFINED; // KEY_VOLUMEUP
      mLocalKeymap[73] = KEY_UNDEFINED; // KEY_VOLUMEDOWN
      mLocalKeymap[74] = KEY_UNDEFINED; // KEY_MUTE
      mLocalKeymap[75] = KEY_KP_DIVIDE;
      mLocalKeymap[76] = KEY_KP_ENTER;
      mLocalKeymap[77] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[78] = KEY_KP_SUBTRACT;
      mLocalKeymap[79] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[80] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[81] = KEY_KP_EQUAL;
      mLocalKeymap[82] = KEY_UNDEFINED; // KEY_KP_0
      mLocalKeymap[83] = KEY_UNDEFINED; // KEY_KP_1
      mLocalKeymap[84] = KEY_UNDEFINED; // KEY_KP_2
      mLocalKeymap[85] = KEY_UNDEFINED; // KEY_KP_3
      mLocalKeymap[86] = KEY_UNDEFINED; // KEY_KP_4
      mLocalKeymap[87] = KEY_UNDEFINED; // KEY_KP_5
      mLocalKeymap[88] = KEY_UNDEFINED; // KEY_KP_6
      mLocalKeymap[89] = KEY_UNDEFINED; // KEY_KP_7
      mLocalKeymap[90] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[91] = KEY_UNDEFINED; // KEY_KP_8
      mLocalKeymap[92] = KEY_UNDEFINED; // KEY_KP_9
      mLocalKeymap[93] = KEY_UNDEFINED; // KEY_INTERNATIONAL3
      mLocalKeymap[94] = KEY_UNDEFINED; // KEY_INTERNATIONAL1
      mLocalKeymap[95] = KEY_UNDEFINED; // KEY_KP_COMMA
      mLocalKeymap[96] = KEY_F5;
      mLocalKeymap[97] = KEY_F6;
      mLocalKeymap[98] = KEY_F7;
      mLocalKeymap[99] = KEY_F3;
      mLocalKeymap[100] = KEY_F8;
      mLocalKeymap[101] = KEY_F9;
      mLocalKeymap[102] = KEY_UNDEFINED; // KEY_LANG2
      mLocalKeymap[103] = KEY_F11;
      mLocalKeymap[104] = KEY_UNDEFINED; // KEY_LANG1
      mLocalKeymap[105] = KEY_PRINTSCREEN;
      mLocalKeymap[106] = KEY_UNDEFINED; // KEY_F16
      mLocalKeymap[107] = KEY_UNDEFINED; // KEY_SCROLLLOCK
      mLocalKeymap[108] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[109] = KEY_F10;
      mLocalKeymap[110] = KEY_UNDEFINED; // KEY_APPLICATION
      mLocalKeymap[111] = KEY_F12;
      mLocalKeymap[112] = KEY_UNDEFINED; // KEY_UNKNOWN
      mLocalKeymap[113] = KEY_PAUSE;
      mLocalKeymap[114] = KEY_INSERT;
      mLocalKeymap[115] = KEY_HOME;
      mLocalKeymap[116] = KEY_PAGEUP;
      mLocalKeymap[117] = KEY_DELETE;
      mLocalKeymap[118] = KEY_F4;
      mLocalKeymap[119] = KEY_END;
      mLocalKeymap[120] = KEY_F2;
      mLocalKeymap[121] = KEY_PAGEDOWN;
      mLocalKeymap[122] = KEY_F1;
      mLocalKeymap[123] = KEY_LEFT;
      mLocalKeymap[124] = KEY_RIGHT;
      mLocalKeymap[125] = KEY_DOWN;
      mLocalKeymap[126] = KEY_UP;
      mLocalKeymap[127] = KEY_UNDEFINED; // KEY_POWER
		}
	}

	Platform::~Platform() {
		CocoaPlatform::instance->KillPlatformCocoa();
		SafeDealloc(CocoaPlatform::instance);
	}

	bool Platform::Initialize(const PlatformContext& param) {
		return CocoaPlatform::instance->CreatePlatformCocoa(
        param.name, param.width, param.height, param.fullscreen, param.majorVersion,
        param.minorVersion, param.multisample, param.style, param.profile);
	}

	void Platform::Update() {
		NSEvent* event;
		do {
			event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
			[NSApp sendEvent: event];
		}
		while(event != nil);
	}

	void Platform::SwapBuffer() {
		[[NSOpenGLContext currentContext] flushBuffer];
	}

	void Platform::WindowSizeChanged(int width, int height) {
		mWidth = width;
		mHeight = height;
		glViewport(0, 0, mWidth, mHeight);
	}

	const PointCoord Platform::CenterOnWindow() {
		int w,h;
		GetDesktopSize(w, h);

		return PointCoord((w - mWidth) / 2, (h - mHeight) / 2);
	}

	void Platform::GetDesktopSize(int& width, int& height) {
		width = static_cast<int>([[NSScreen mainScreen] frame].size.width);
		height = static_cast<int>([[NSScreen mainScreen] frame].size.height);
	}

	void Platform::SetMousePos(const int x, const int y) const {
    // not work
    Debug::Log("this feature does not support for MacOS");
	}

	int Platform::GetScreenDPI() {
	  // TODO: test this method
    NSScreen* screen = [NSScreen mainScreen];
    NSDictionary* description = [screen deviceDescription];
    NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
    CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);
		return (displayPixelSize.width / displayPhysicalSize.width) * 25.4f;
	}

	void Platform::SetVSync(bool sync) {
	  // not work
    GLint syncValue = static_cast<GLint>(sync);
    [[NSOpenGLContext currentContext] setValues:&syncValue forParameter:NSOpenGLCPSwapInterval];
	}

	int Platform::GetVSync() {
    // not work
    GLint syncValue = 0;
    [[NSOpenGLContext currentContext] getValues:&syncValue forParameter:NSOpenGLCPSwapInterval];
		return syncValue;
	}

	bool Platform::IsFocus() const {
		return mIsFocused;
	}
  
  void Platform::ChangeTitle(const std::string& titleName) {
    [Theodore::CocoaPlatform::GetInstance()->window setTitle: CocoaPlatform::toNSString(titleName)];
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cocoa objective-c definition

@implementation CocoaWindow
- (BOOL)canBecomeKeyWindow
{
  return YES;
}

- (BOOL)canBecomeMainWindow
{
  return YES;
}
@end

@implementation View
- (void)windowDidResize:(NSNotification *)notification {
  NSSize size = [[_window contentView] frame].size;
  Theodore::Platform::GetInstance()->WindowSizeChanged(size.width, size.height);
}
// terminate window when the red X is pressed
- (void)windowWillClose:(NSNotification *)notification {
  Theodore::Platform::GetInstance()->Quit();
  Theodore::CocoaPlatform::GetInstance()->KillPlatformCocoa();
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (void)windowDidBecomeKey:(NSEvent *)event {
	Theodore::Platform::GetInstance()->mIsFocused = true;
}

- (void)windowDidResignKey:(NSEvent *)event {
	Theodore::Platform::GetInstance()->mIsFocused = false;
}

- (void)keyDown:(NSEvent *)event {
  unsigned short scancode = [event keyCode];
  if (scancode < 128) {
    unsigned short localKey = Theodore::Platform::GetInstance()->mLocalKeymap[scancode];
    if (localKey != -1)
      Theodore::Platform::GetInstance()->mKeys[localKey] = true;
  }
}

- (void)keyUp:(NSEvent *)event {
  unsigned short scancode = [event keyCode];
  if (scancode < 128) {
    unsigned short localKey = Theodore::Platform::GetInstance()->mLocalKeymap[scancode];
    if (localKey != -1)
      Theodore::Platform::GetInstance()->mKeys[localKey] = false;
  }
}

- (void)flagsChanged:(NSEvent *)event {
  unsigned short scancode = [event keyCode];
  unsigned short localKey = Theodore::Platform::GetInstance()->mLocalKeymap[scancode];
  if (localKey != -1) {
    Theodore::Platform::GetInstance()->mKeys[localKey] = !Theodore::Platform::GetInstance()->mKeys[localKey];
  }
}

- (void)scrollWheel:(NSEvent *)event {
	// horizontall [event deltaX], verticall [event deltaY]
  Theodore::Platform::GetInstance()->mMousePosition.z = [event deltaY];
}

- (void)mouseMoved:(NSEvent *)event {
  NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
  Theodore::Platform::GetInstance()->mMousePosition.x = point.x;
  Theodore::Platform::GetInstance()->mMousePosition.y = point.y;
}

- (void)mouseDown:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mMouseButtons[Theodore::MOUSE_LEFT] = true;
}

- (void)mouseUp:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mMouseButtons[Theodore::MOUSE_LEFT] = false;
}

- (void)rightMouseDown:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mMouseButtons[Theodore::MOUSE_RIGHT] = true;
}

- (void)rightMouseUp:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mMouseButtons[Theodore::MOUSE_RIGHT] = false;
}

- (void)otherMouseDown:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mMouseButtons[Theodore::MOUSE_MIDDLE] = true;
}

- (void)otherMouseUp:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mMouseButtons[Theodore::MOUSE_MIDDLE] = false;
}
@end

#endif /* Theodore_MacOS */
