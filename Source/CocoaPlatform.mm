#include "os_types.h"

#ifdef Theodore_MacOS

// the including order is matters!
#include GLEW_INCLUDE_DIR
#import <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "Platform.h"
#include "CocoaPlatform.h"
#include "Utility.h"
#include "KeyCode.h"
#include "Debug.h"

namespace Theodore {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // CocoaPlatform definition

    CocoaPlatform* CocoaPlatform::instance = NULL;
    Platform* CocoaPlatform::platform = NULL;
    CocoaPlatform::CocoaPlatform() :window(NULL), view(NULL) {

    }

    CocoaPlatform::~CocoaPlatform() {

    }

    bool CocoaPlatform::CreatePlatformCocoa(const std::string& title, int width, int height, bool fullscreen, int majorVersion, int minorVersion, int multisample, WindowStyle style, ContextProfile profile) {
        @autoreleasepool {
            platform->mWidth = width;
            platform->mHeight = height;
            platform->mTitle = title;

            [NSApplication sharedApplication];
            const PointCoord pos = platform->CenterOnWindow();
            NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable| NSWindowStyleMaskResizable;
            NSRect windowRect = NSMakeRect(pos.x, pos.y, width, height); // x, y, w, h
            window = [[NSWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];

            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

            // convert from CString to NSString.
            NSString* title_name = [NSString stringWithCString:title.c_str() encoding:[NSString defaultCStringEncoding]];

            NSMenu *appleMenu;
            NSString* title;
            NSMenuItem *menuItem;

            /* Create the main menu bar */
            [NSApp setMainMenu:[[NSMenu alloc] init]];

            /* Create the application menu */
            appleMenu = [[NSMenu alloc] initWithTitle:@""];

            /* Add menu items */
            title = [@"About " stringByAppendingString:title_name];
            [appleMenu addItemWithTitle:title action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];

            title = [@"Hide " stringByAppendingString:title_name];
            [appleMenu addItemWithTitle:title action:@selector(hide:) keyEquivalent:@"h"];

            menuItem = (NSMenuItem *)[appleMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"];
            [menuItem setKeyEquivalentModifierMask:(NSEventModifierFlagOption|NSEventModifierFlagCommand)];

            [appleMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];

            [appleMenu addItem:[NSMenuItem separatorItem]];

            title = [@"Quit " stringByAppendingString:title_name];
            [appleMenu addItemWithTitle:title action:@selector(terminate:) keyEquivalent:@"q"];

            /* Put menu into the menubar */
            menuItem = [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
            [menuItem setSubmenu:appleMenu];
            [[NSApp mainMenu] addItem:menuItem];

            /* Tell the application object that this is now the application menu */
            [NSApp setMainMenu:appleMenu];

            // no multisampling
            int samples = 0;

            NSOpenGLPixelFormatAttribute windowedAttrs[] =
            {
                NSOpenGLPFAMultisample,
                NSOpenGLPFASampleBuffers, static_cast<NSOpenGLPixelFormatAttribute>(samples ? 1 : 0),
                NSOpenGLPFASamples, static_cast<NSOpenGLPixelFormatAttribute>(samples),
                NSOpenGLPFAAccelerated,
                NSOpenGLPFADoubleBuffer,
                NSOpenGLPFAColorSize, 24,
                NSOpenGLPFADepthSize, 24,
                NSOpenGLPFAAlphaSize, 8,
                NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, //NSOpenGLProfileVersionLegacy,
                0
            };

            // try to choose a supported pixel format
            NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:windowedAttrs];
            if(!pf) return false;

            view = [[View alloc] initWithFrame:windowRect pixelFormat:pf];
            [[view window] setLevel:NSNormalWindowLevel];
            [[view window] makeKeyAndOrderFront:window];
            glewExperimental = GL_TRUE;

            // make all the OpenGL calls to setup rendering and build the necessary rendering objects.
            [[view openGLContext] makeCurrentContext];

            GLint swapInt = 1;
            [[view openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
            if(glewInit() != GLEW_OK) return false;

            Debug::Log("Vendor              : %s\n", glGetString(GL_VENDOR));
            Debug::Log("Renderer            : %s\n", glGetString(GL_RENDERER));
            Debug::Log("Version             : %s\n", glGetString(GL_VERSION));
            Debug::Log("GLSL                : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

            [window setAcceptsMouseMovedEvents:YES];
            [window setContentView:view];
            [window setDelegate:view];

            [window setTitle:title_name];
            [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
            [window orderFrontRegardless];

            return true;
        }
    }

    void CocoaPlatform::KillPlatformCocoa() {
        [view clearGLContext];
        [NSApp terminate:window];
    }

    CocoaPlatform* CocoaPlatform::GetInstance() {
        return CocoaPlatform::instance;
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
            mLocalKeymap[KEY_A] = kVK_ANSI_A;
            mLocalKeymap[KEY_B] = kVK_ANSI_B;
            mLocalKeymap[KEY_C] = kVK_ANSI_C;
            mLocalKeymap[KEY_D] = kVK_ANSI_D;
            mLocalKeymap[KEY_E] = kVK_ANSI_E;
            mLocalKeymap[KEY_F] = kVK_ANSI_F;
            mLocalKeymap[KEY_G] = kVK_ANSI_G;
            mLocalKeymap[KEY_H] = kVK_ANSI_H;
            mLocalKeymap[KEY_I] = kVK_ANSI_I;
            mLocalKeymap[KEY_J] = kVK_ANSI_J;
            mLocalKeymap[KEY_K] = kVK_ANSI_K;
            mLocalKeymap[KEY_L] = kVK_ANSI_L;
            mLocalKeymap[KEY_M] = kVK_ANSI_M;
            mLocalKeymap[KEY_N] = kVK_ANSI_N;
            mLocalKeymap[KEY_O] = kVK_ANSI_O;
            mLocalKeymap[KEY_P] = kVK_ANSI_P;
            mLocalKeymap[KEY_Q] = kVK_ANSI_Q;
            mLocalKeymap[KEY_R] = kVK_ANSI_R;
            mLocalKeymap[KEY_S] = kVK_ANSI_S;
            mLocalKeymap[KEY_T] = kVK_ANSI_T;
            mLocalKeymap[KEY_U] = kVK_ANSI_U;
            mLocalKeymap[KEY_V] = kVK_ANSI_V;
            mLocalKeymap[KEY_W] = kVK_ANSI_W;
            mLocalKeymap[KEY_X] = kVK_ANSI_X;
            mLocalKeymap[KEY_Y] = kVK_ANSI_Y;
            mLocalKeymap[KEY_Z] = kVK_ANSI_Z;

            mLocalKeymap[KEY_0] = kVK_ANSI_0;
            mLocalKeymap[KEY_1] = kVK_ANSI_1;
            mLocalKeymap[KEY_2] = kVK_ANSI_2;
            mLocalKeymap[KEY_3] = kVK_ANSI_3;
            mLocalKeymap[KEY_4] = kVK_ANSI_4;
            mLocalKeymap[KEY_5] = kVK_ANSI_5;
            mLocalKeymap[KEY_6] = kVK_ANSI_6;
            mLocalKeymap[KEY_7] = kVK_ANSI_7;
            mLocalKeymap[KEY_8] = kVK_ANSI_8;
            mLocalKeymap[KEY_9] = kVK_ANSI_9;

            mLocalKeymap[KEY_SPACE] = kVK_Space;
            mLocalKeymap[KEY_SEMICOLON] = kVK_ANSI_Semicolon;
            mLocalKeymap[KEY_EQUALS] = kVK_ANSI_Equal;
            mLocalKeymap[KEY_APOSTROPHE] = kVK_ANSI_Quote;
            mLocalKeymap[KEY_COMMA] = kVK_ANSI_Comma;
            mLocalKeymap[KEY_MINUS] = kVK_ANSI_Minus;
            mLocalKeymap[KEY_PERIOD] = kVK_ANSI_Period;
            mLocalKeymap[KEY_SLASH] = kVK_ANSI_Slash;
            mLocalKeymap[KEY_LEFTBRACKET] = kVK_ANSI_LeftBracket;
            mLocalKeymap[KEY_BACKSLASH] = kVK_ANSI_Backslash;
            mLocalKeymap[KEY_RIGHTBRACKET] = kVK_ANSI_RightBracket;
            mLocalKeymap[KEY_GRAVE_ACCENT] = kVK_ANSI_Grave;

            mLocalKeymap[KEY_ESCAPE] = kVK_Escape;
            mLocalKeymap[KEY_RETURN] = kVK_Return;
            mLocalKeymap[KEY_TAB] = kVK_Tab;
            mLocalKeymap[KEY_BACKSPACE] = kVK_ForwardDelete;
            mLocalKeymap[KEY_INSERT] = kVK_Help;
            mLocalKeymap[KEY_DELETE] = kVK_Delete;
            mLocalKeymap[KEY_UP] = kVK_UpArrow;
            mLocalKeymap[KEY_DOWN] = kVK_DownArrow;
            mLocalKeymap[KEY_RIGHT] = kVK_RightArrow;
            mLocalKeymap[KEY_LEFT] = kVK_LeftArrow;
            mLocalKeymap[KEY_PAGEUP] = kVK_PageUp;
            mLocalKeymap[KEY_PAGEDOWN] = kVK_PageDown;
            mLocalKeymap[KEY_HOME] = kVK_Home;
            mLocalKeymap[KEY_END] = kVK_End;
            mLocalKeymap[KEY_CAPSLOCK] = kVK_CapsLock;
            mLocalKeymap[KEY_SCROLLOCK] = kVK_F14; // F14
            mLocalKeymap[KEY_NUMLOCK] = kVK_ANSI_KeypadClear;
            mLocalKeymap[KEY_PRINTSCREEN] = kVK_F13; // F13
            mLocalKeymap[KEY_PAUSE] = kVK_F15; // F15

            mLocalKeymap[KEY_F1] = kVK_F1;
            mLocalKeymap[KEY_F2] = kVK_F2;
            mLocalKeymap[KEY_F3] = kVK_F3;
            mLocalKeymap[KEY_F4] = kVK_F4;
            mLocalKeymap[KEY_F5] = kVK_F5;
            mLocalKeymap[KEY_F6] = kVK_F6;
            mLocalKeymap[KEY_F7] = kVK_F7;
            mLocalKeymap[KEY_F8] = kVK_F8;
            mLocalKeymap[KEY_F9] = kVK_F9;
            mLocalKeymap[KEY_F10] = kVK_F10;
            mLocalKeymap[KEY_F11] = kVK_F11;
            mLocalKeymap[KEY_F12] = kVK_F12;

            mLocalKeymap[KEY_LSHIFT] = kVK_Shift;
            mLocalKeymap[KEY_LCTRL] = kVK_Control;
            mLocalKeymap[KEY_LALT] = kVK_Option;
            mLocalKeymap[KEY_LSUPER] = kVK_Command;
            mLocalKeymap[KEY_RSHIFT] = kVK_RightShift;
            mLocalKeymap[KEY_RCTRL] = kVK_RightControl;
            mLocalKeymap[KEY_RALT] = kVK_RightOption;
            mLocalKeymap[KEY_RSUPER] = kVK_RightCommand;
            mLocalKeymap[KEY_MENU] = kVK_Function; // maybe?
            mLocalKeymap[KEY_HANGUL] = KEY_UNDEFINED;
            mLocalKeymap[KEY_HANJA] = KEY_UNDEFINED;

            mLocalKeymap[KEY_KP_0] = kVK_ANSI_Keypad0;
            mLocalKeymap[KEY_KP_1] = kVK_ANSI_Keypad1;
            mLocalKeymap[KEY_KP_2] = kVK_ANSI_Keypad2;
            mLocalKeymap[KEY_KP_3] = kVK_ANSI_Keypad3;
            mLocalKeymap[KEY_KP_4] = kVK_ANSI_Keypad4;
            mLocalKeymap[KEY_KP_5] = kVK_ANSI_Keypad5;
            mLocalKeymap[KEY_KP_6] = kVK_ANSI_Keypad6;
            mLocalKeymap[KEY_KP_7] = kVK_ANSI_Keypad7;
            mLocalKeymap[KEY_KP_8] = kVK_ANSI_Keypad8;
            mLocalKeymap[KEY_KP_9] = kVK_ANSI_Keypad9;

            mLocalKeymap[KEY_KP_DECIMAL] = kVK_ANSI_KeypadDecimal;
            mLocalKeymap[KEY_KP_DIVIDE] = kVK_ANSI_KeypadDivide;
            mLocalKeymap[KEY_KP_MULTIPLY] = kVK_ANSI_KeypadMultiply;
            mLocalKeymap[KEY_KP_SUBTRACT] = kVK_ANSI_KeypadMinus;
            mLocalKeymap[KEY_KP_ADD] = kVK_ANSI_KeypadPlus;
            mLocalKeymap[KEY_KP_ENTER] = kVK_ANSI_KeypadEnter;
            mLocalKeymap[KEY_KP_EQUAL] = kVK_ANSI_KeypadEquals;
        }
    }

    Platform::~Platform() {
        CocoaPlatform::instance->KillPlatformCocoa();
        SafeDealloc(CocoaPlatform::instance);
    }

    bool Platform::Initialize(const PlatformContext& param) {
        return CocoaPlatform::instance->CreatePlatformCocoa(param.name, param.width, param.height, param.fullscreen, param.majorVersion, param.minorVersion, param.multisample, param.style, param.profile);
    }

    void Platform::Update() {
        NSEvent * event;
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

    }

    int Platform::GetScreenDPI() {
        return 0;
    }

    void Platform::SetVSync(bool sync) {

    }

    int Platform::GetVSync() {
        return 0;
    }

    bool Platform::IsFocus() const {
        return true;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cocoa objective-c definition

@implementation View
- (void)windowDidResize:(NSNotification *)notification {
    NSSize size = [[_window contentView] frame].size;
    Quark::Platform::GetInstance()->WindowSizeChanged(size.width, size.height);
}
// terminate window when the red X is pressed
- (void)windowWillClose:(NSNotification *)notification {
    Quark::Platform::GetInstance()->Quit();
    Quark::CocoaPlatform::GetInstance()->KillPlatformCocoa();
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
    return YES;
}

- (void)keyDown:(NSEvent *)event {
    NSLog(@"event monitor: %@", event);
    Quark::Platform::GetInstance()->mKeys[[event keyCode]] = true;
}

- (void)keyUp:(NSEvent *)event {
    Quark::Platform::GetInstance()->mKeys[[event keyCode]] = false;
}

- (void)scrollWheel:(NSEvent *)event {
    // horizontall [event deltaX], verticall [event deltaY]
    Quark::Platform::GetInstance()->mMousePosition.z = [event deltaY];
}

- (void)mouseMoved:(NSEvent *)event {
    NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    Quark::Platform::GetInstance()->mMousePosition.x = point.x;
    Quark::Platform::GetInstance()->mMousePosition.y = point.y;
}

- (void)mouseDown:(NSEvent *)event {
    Quark::Platform::GetInstance()->mMouseButtons[Quark::MOUSE_LEFT] = true;
}

- (void)mouseUp:(NSEvent *)event {
    Quark::Platform::GetInstance()->mMouseButtons[Quark::MOUSE_LEFT] = false;
}

- (void)rightMouseDown:(NSEvent *)event {
    Quark::Platform::GetInstance()->mMouseButtons[Quark::MOUSE_RIGHT] = true;
}

- (void)rightMouseUp:(NSEvent *)event {
    Quark::Platform::GetInstance()->mMouseButtons[Quark::MOUSE_RIGHT] = false;
}

- (void)otherMouseDown:(NSEvent *)event {
    Quark::Platform::GetInstance()->mMouseButtons[Quark::MOUSE_MIDDLE] = true;
}

- (void)otherMouseUp:(NSEvent *)event {
    Quark::Platform::GetInstance()->mMouseButtons[Quark::MOUSE_MIDDLE] = false;
}
@end

#endif /* Theodore_MacOS */
