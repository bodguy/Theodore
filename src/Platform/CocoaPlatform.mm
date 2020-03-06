// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "CondiitonalVars.h"

#ifdef Theodore_MacOS

// the including order is matters!
#include GLEW_INCLUDE_DIR
#import <Cocoa/Cocoa.h>
#include "Platform.h"
#include "CocoaPlatform.h"
#include "Helper/Utility.h"

namespace Theodore {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CocoaPlatform definition

	CocoaPlatform* CocoaPlatform::instance = NULL;
	Platform* CocoaPlatform::platform = NULL;
	CocoaPlatform::CocoaPlatform() :window(NULL), view(NULL) {}

	CocoaPlatform::~CocoaPlatform() {}

	bool CocoaPlatform::CreatePlatformCocoa(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, WindowStyle style, ContextProfile profile) {
		@autoreleasepool {
      platform->title = title;
			platform->width = width;
			platform->height = height;
      platform->isFullScreen = fullscreen;

			[NSApplication sharedApplication];
      NSUInteger windowStyle = 0;
      NSRect windowRect;

			if (platform->isFullScreen) {
        windowRect = [[NSScreen mainScreen] frame];
        windowStyle = NSWindowStyleMaskBorderless;
        window = [[CocoaWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
        [window setLevel:NSMainMenuWindowLevel+1];
        platform->isCursorVisible = false;
			} else {
        const PointCoord pos = platform->CenterOnWindow();
        windowRect = NSMakeRect(pos.x, pos.y, platform->width, platform->height); // x, y, w, h

        windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
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

			[window setTitle: CocoaPlatform::toNSString(platform->title)];
      [window makeKeyAndOrderFront:nil];
			[window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
			[window orderFrontRegardless];

			if (platform->isCursorVisible) {
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
    platform->isMultisampleSupported = multisample ? true : false;

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
    [[view openGLContext] setValues:&swapInt forParameter:NSOpenGLContextParameterSwapInterval];

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
	Platform::Platform() : width(0), height(0), isCursorVisible(true), isRunning(true), mousePosition() {
		CocoaPlatform::instance = new CocoaPlatform();
		instance = this;
		CocoaPlatform::instance->platform = this;

		for (int i = 0; i < KEY_MAX; i++) {
			keys[i] = false;
			localKeymap[i] = KEY_UNDEFINED;
		}

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
			mouseButtons[i] = false;

		{ // optimized to Apple Extended Keyboard II (domestic layout)
			localKeymap[0] = KEY_A;
			localKeymap[1] = KEY_S;
			localKeymap[2] = KEY_D;
			localKeymap[3] = KEY_F;
			localKeymap[4] = KEY_H;
			localKeymap[5] = KEY_G;
			localKeymap[6] = KEY_Z;
			localKeymap[7] = KEY_X;
			localKeymap[8] = KEY_C;
			localKeymap[9] = KEY_V;
			localKeymap[10] = KEY_UNDEFINED; // for historical reason
			localKeymap[11] = KEY_B;
			localKeymap[12] = KEY_Q;
			localKeymap[13] = KEY_W;
			localKeymap[14] = KEY_E;
			localKeymap[15] = KEY_R;
			localKeymap[16] = KEY_Y;
			localKeymap[17] = KEY_T;
			localKeymap[18] = KEY_1;
			localKeymap[19] = KEY_2;
			localKeymap[20] = KEY_3;
			localKeymap[21] = KEY_4;
			localKeymap[22] = KEY_6;
			localKeymap[23] = KEY_5;
			localKeymap[24] = KEY_EQUALS;
			localKeymap[25] = KEY_9;
			localKeymap[26] = KEY_7;
			localKeymap[27] = KEY_MINUS;
			localKeymap[28] = KEY_8;
			localKeymap[29] = KEY_0;
			localKeymap[30] = KEY_RIGHTBRACKET;
			localKeymap[31] = KEY_O;
			localKeymap[32] = KEY_U;
			localKeymap[33] = KEY_LEFTBRACKET;
			localKeymap[34] = KEY_I;
			localKeymap[35] = KEY_P;
			localKeymap[36] = KEY_RETURN;
			localKeymap[37] = KEY_L;
			localKeymap[38] = KEY_J;
			localKeymap[39] = KEY_APOSTROPHE;
			localKeymap[40] = KEY_K;
			localKeymap[41] = KEY_SEMICOLON;
			localKeymap[42] = KEY_BACKSLASH;
			localKeymap[43] = KEY_COMMA;
			localKeymap[44] = KEY_SLASH;
			localKeymap[45] = KEY_N;
			localKeymap[46] = KEY_M;
			localKeymap[47] = KEY_PERIOD;
			localKeymap[48] = KEY_TAB;
			localKeymap[49] = KEY_SPACE;
			localKeymap[50] = KEY_GRAVE_ACCENT;
			localKeymap[51] = KEY_BACKSPACE;
			localKeymap[52] = KEY_KP_ENTER;
			localKeymap[53] = KEY_ESCAPE;
			localKeymap[54] = KEY_RSUPER;
			localKeymap[55] = KEY_LSUPER;
			localKeymap[56] = KEY_LSHIFT;
			localKeymap[57] = KEY_CAPSLOCK;
			localKeymap[58] = KEY_LALT;
			localKeymap[59] = KEY_LCTRL;
			localKeymap[60] = KEY_RSHIFT;
			localKeymap[61] = KEY_RALT;
			localKeymap[62] = KEY_RCTRL;
			localKeymap[63] = KEY_UNDEFINED; // KEY_RGUI
      localKeymap[64] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[65] = KEY_UNDEFINED; // KEY_KP_PERIOD
      localKeymap[66] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[67] = KEY_KP_MULTIPLY;
			localKeymap[68] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[69] = KEY_KP_ADD;
			localKeymap[70] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[71] = KEY_UNDEFINED; // KEY_NUMLOCKCLEAR
      localKeymap[72] = KEY_UNDEFINED; // KEY_VOLUMEUP
      localKeymap[73] = KEY_UNDEFINED; // KEY_VOLUMEDOWN
      localKeymap[74] = KEY_UNDEFINED; // KEY_MUTE
      localKeymap[75] = KEY_KP_DIVIDE;
			localKeymap[76] = KEY_KP_ENTER;
			localKeymap[77] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[78] = KEY_KP_SUBTRACT;
			localKeymap[79] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[80] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[81] = KEY_KP_EQUAL;
			localKeymap[82] = KEY_UNDEFINED; // KEY_KP_0
      localKeymap[83] = KEY_UNDEFINED; // KEY_KP_1
      localKeymap[84] = KEY_UNDEFINED; // KEY_KP_2
      localKeymap[85] = KEY_UNDEFINED; // KEY_KP_3
      localKeymap[86] = KEY_UNDEFINED; // KEY_KP_4
      localKeymap[87] = KEY_UNDEFINED; // KEY_KP_5
      localKeymap[88] = KEY_UNDEFINED; // KEY_KP_6
      localKeymap[89] = KEY_UNDEFINED; // KEY_KP_7
      localKeymap[90] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[91] = KEY_UNDEFINED; // KEY_KP_8
      localKeymap[92] = KEY_UNDEFINED; // KEY_KP_9
      localKeymap[93] = KEY_UNDEFINED; // KEY_INTERNATIONAL3
      localKeymap[94] = KEY_UNDEFINED; // KEY_INTERNATIONAL1
      localKeymap[95] = KEY_UNDEFINED; // KEY_KP_COMMA
      localKeymap[96] = KEY_F5;
			localKeymap[97] = KEY_F6;
			localKeymap[98] = KEY_F7;
			localKeymap[99] = KEY_F3;
			localKeymap[100] = KEY_F8;
			localKeymap[101] = KEY_F9;
			localKeymap[102] = KEY_UNDEFINED; // KEY_LANG2
      localKeymap[103] = KEY_F11;
			localKeymap[104] = KEY_UNDEFINED; // KEY_LANG1
      localKeymap[105] = KEY_PRINTSCREEN;
			localKeymap[106] = KEY_UNDEFINED; // KEY_F16
      localKeymap[107] = KEY_UNDEFINED; // KEY_SCROLLLOCK
      localKeymap[108] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[109] = KEY_F10;
			localKeymap[110] = KEY_UNDEFINED; // KEY_APPLICATION
      localKeymap[111] = KEY_F12;
			localKeymap[112] = KEY_UNDEFINED; // KEY_UNKNOWN
      localKeymap[113] = KEY_PAUSE;
			localKeymap[114] = KEY_INSERT;
			localKeymap[115] = KEY_HOME;
			localKeymap[116] = KEY_PAGEUP;
			localKeymap[117] = KEY_DELETE;
			localKeymap[118] = KEY_F4;
			localKeymap[119] = KEY_END;
			localKeymap[120] = KEY_F2;
			localKeymap[121] = KEY_PAGEDOWN;
			localKeymap[122] = KEY_F1;
			localKeymap[123] = KEY_LEFT;
			localKeymap[124] = KEY_RIGHT;
			localKeymap[125] = KEY_DOWN;
			localKeymap[126] = KEY_UP;
			localKeymap[127] = KEY_UNDEFINED; // KEY_POWER
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
		width = width;
		height = height;
		glViewport(0, 0, width, height);
	}

	const PointCoord Platform::CenterOnWindow() {
		int w,h;
		GetDesktopSize(w, h);

		return PointCoord((w - width) / 2.f, (h - height) / 2.f);
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
    [[NSOpenGLContext currentContext] setValues:&syncValue forParameter:NSOpenGLContextParameterSwapInterval];
	}

	int Platform::GetVSync() {
    // not work
    GLint syncValue = 0;
    [[NSOpenGLContext currentContext] getValues:&syncValue forParameter:NSOpenGLContextParameterSwapInterval];
		return syncValue;
	}

	bool Platform::IsFocus() const {
		return isFocused;
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

- (CVReturn)GetFrameForTime:(const CVTimeStamp *)outputTime {
  return kCVReturnSuccess;
}

- (void)windowDidResize:(NSNotification *)notification {
  NSSize size = [[self.window contentView] frame].size;
  Theodore::Platform::GetInstance()->WindowSizeChanged(size.width, size.height);
}
// terminate window when the red X is pressed
- (void)windowWillClose:(NSNotification *)notification {
  Theodore::Platform::GetInstance()->Quit();
  Theodore::CocoaPlatform::GetInstance()->KillPlatformCocoa();
//  CVDisplayLinkStop(displayLink);
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (void)windowDidBecomeKey:(NSEvent *)event {
	Theodore::Platform::GetInstance()->isFocused = true;
}

- (void)windowDidResignKey:(NSEvent *)event {
	Theodore::Platform::GetInstance()->isFocused = false;
}

- (void)keyDown:(NSEvent *)event {
  unsigned short scancode = [event keyCode];
  if (scancode < 128) {
    unsigned short localKey = Theodore::Platform::GetInstance()->localKeymap[scancode];
    if (localKey != -1)
      Theodore::Platform::GetInstance()->keys[localKey] = true;
  }
}

- (void)keyUp:(NSEvent *)event {
  unsigned short scancode = [event keyCode];
  if (scancode < 128) {
    unsigned short localKey = Theodore::Platform::GetInstance()->localKeymap[scancode];
    if (localKey != -1)
      Theodore::Platform::GetInstance()->keys[localKey] = false;
  }
}

- (void)flagsChanged:(NSEvent *)event {
  unsigned short scancode = [event keyCode];
  unsigned short localKey = Theodore::Platform::GetInstance()->localKeymap[scancode];
  if (localKey != -1) {
    Theodore::Platform::GetInstance()->keys[localKey] = !Theodore::Platform::GetInstance()->keys[localKey];
  }
}

- (void)scrollWheel:(NSEvent *)event {
	// currently ignoring deltaX (TODO)
	double deltaX = [event scrollingDeltaX];
	double deltaY = [event scrollingDeltaY];

	if ([event hasPreciseScrollingDeltas]) {
	  deltaX *= 0.1;
    deltaY *= 0.1;
	}

	if (fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0) {
    Theodore::Platform::GetInstance()->mousePosition.z = deltaY;
	}
}

- (void)mouseMoved:(NSEvent *)event {
  NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
  Theodore::Platform::GetInstance()->mousePosition.x = point.x;
  Theodore::Platform::GetInstance()->mousePosition.y = point.y;
}

- (void)mouseDown:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mouseButtons[Theodore::MOUSE_LEFT] = true;
}

- (void)mouseUp:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mouseButtons[Theodore::MOUSE_LEFT] = false;
}

- (void)rightMouseDown:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mouseButtons[Theodore::MOUSE_RIGHT] = true;
}

- (void)rightMouseUp:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mouseButtons[Theodore::MOUSE_RIGHT] = false;
}

- (void)otherMouseDown:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mouseButtons[Theodore::MOUSE_MIDDLE] = true;
}

- (void)otherMouseUp:(NSEvent *)event {
  Theodore::Platform::GetInstance()->mouseButtons[Theodore::MOUSE_MIDDLE] = false;
}

- (void)mouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}

- (void)rightMouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}

- (void)otherMouseDragged:(NSEvent *)event
{
  [self mouseMoved:event];
}
@end

#endif /* Theodore_MacOS */
