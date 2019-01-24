#include "os_types.h"

#ifdef Theodore_Linux

// the including order is matters!
#include <X11/Xlib.h>
#include GLEW_INCLUDE_DIR
#include GLEW_INCLUDE_DIR2
#include "../Helper/Utility.h"
#include "LinuxPlatform.h"
#include "Platform.h"

namespace Theodore {

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // LinuxPlatform definition

  LinuxPlatform* LinuxPlatform::instance = NULL;
  Platform* LinuxPlatform::platform = NULL;
  LinuxPlatform::LinuxPlatform() : mDisplay(NULL), mWindow(0), mScreen(0), mContext(NULL) {}

  LinuxPlatform::~LinuxPlatform() {}

  bool LinuxPlatform::CreatePlatformLinux(const std::string& title, int width, int height,
                                          bool fullscreen, int majorVersion, int minorVersion,
                                          int multisample, WindowStyle style,
                                          ContextProfile profile) {
    platform->mWidth = width;
    platform->mHeight = height;
    platform->mTitle = title;
    platform->mIsFullScreen = fullscreen;

    mDisplay = XOpenDisplay(NULL);
    if (!mDisplay)
      return false;

    // get default window, screen
    Window rootWindow = DefaultRootWindow(mDisplay);
    mScreen = DefaultScreen(mDisplay);

    // set the visual info
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, DefaultDepth(mDisplay, mScreen), GLX_DOUBLEBUFFER,
                   None};
    XVisualInfo* vi = glXChooseVisual(mDisplay, mScreen, att);
    if (!vi)
      return false;

    // set the color map
    Colormap cmap = XCreateColormap(mDisplay, rootWindow, vi->visual, AllocNone);
    if (!cmap)
      return false;
    XSetWindowAttributes attribs;
    attribs.colormap = cmap;
    attribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                         ButtonReleaseMask | PointerMotionMask;

    // create window
    const Point p = platform->CenterOnWindow();
    mWindow = XCreateWindow(mDisplay, rootWindow, static_cast<int>(p.x), static_cast<int>(p.y),
                            width, height, 1, DefaultDepth(mDisplay, mScreen), InputOutput,
                            vi->visual, CWColormap | CWEventMask, &attribs);
    if (!mWindow)
      return false;
    XMapWindow(mDisplay, mWindow);
    XStoreName(mDisplay, mWindow, title.c_str());

    // attach Delete Message
    mDestroyMessage = XInternAtom(mDisplay, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(mDisplay, mWindow, &mDestroyMessage, 1);

    // make GL context
    mContext = glXCreateContext(mDisplay, vi, NULL, GL_TRUE);
    if (!mContext)
      return false;
    glXMakeCurrent(mDisplay, mWindow, mContext);
    if (glewInit() != GLEW_OK)
      return false;

    Debug::Log("Renderer: %s\n", glGetString(GL_RENDERER));
    Debug::Log("OpenGL version supported by this platform (%s) \n", glGetString(GL_VERSION));
    Debug::Log("GLSL version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
  }

  void LinuxPlatform::KillPlatformLinux() {
    glXMakeCurrent(mDisplay, None, NULL);
    glXDestroyContext(mDisplay, mContext);
    XDestroyWindow(mDisplay, mWindow);
    XCloseDisplay(mDisplay);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Platform definition

  Platform* Platform::instance = NULL;
  Platform::Platform()
      : mWidth(0), mHeight(0), mIsShowCursor(true), mIsRunning(true), mMousePosition() {
    LinuxPlatform::instance = new LinuxPlatform();
    instance = this;
    LinuxPlatform::instance->platform = this;

    for (int i = 0; i < KEY_MAX; i++) {
      mKeys[i] = false;
      mLocalKeymap[i] = KEY_UNDEFINED;
    }

    for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
      mMouseButtons[i] = false;

    mLocalKeymap[KEY_A] = XK_a;
    mLocalKeymap[KEY_B] = XK_b;
    mLocalKeymap[KEY_C] = XK_c;
    mLocalKeymap[KEY_D] = XK_d;
    mLocalKeymap[KEY_E] = XK_e;
    mLocalKeymap[KEY_F] = XK_f;
    mLocalKeymap[KEY_G] = XK_g;
    mLocalKeymap[KEY_H] = XK_h;
    mLocalKeymap[KEY_I] = XK_i;
    mLocalKeymap[KEY_J] = XK_j;
    mLocalKeymap[KEY_K] = XK_k;
    mLocalKeymap[KEY_L] = XK_l;
    mLocalKeymap[KEY_M] = XK_m;
    mLocalKeymap[KEY_N] = XK_n;
    mLocalKeymap[KEY_O] = XK_o;
    mLocalKeymap[KEY_P] = XK_p;
    mLocalKeymap[KEY_Q] = XK_q;
    mLocalKeymap[KEY_R] = XK_r;
    mLocalKeymap[KEY_S] = XK_s;
    mLocalKeymap[KEY_T] = XK_t;
    mLocalKeymap[KEY_U] = XK_u;
    mLocalKeymap[KEY_V] = XK_v;
    mLocalKeymap[KEY_W] = XK_w;
    mLocalKeymap[KEY_X] = XK_x;
    mLocalKeymap[KEY_Y] = XK_y;
    mLocalKeymap[KEY_Z] = XK_z;

    mLocalKeymap[KEY_0] = XK_0;
    mLocalKeymap[KEY_1] = XK_1;
    mLocalKeymap[KEY_2] = XK_2;
    mLocalKeymap[KEY_3] = XK_3;
    mLocalKeymap[KEY_4] = XK_4;
    mLocalKeymap[KEY_5] = XK_5;
    mLocalKeymap[KEY_6] = XK_6;
    mLocalKeymap[KEY_7] = XK_7;
    mLocalKeymap[KEY_8] = XK_8;
    mLocalKeymap[KEY_9] = XK_9;

    mLocalKeymap[KEY_SPACE] = XK_space;
    mLocalKeymap[KEY_SEMICOLON] = XK_semicolon;
    mLocalKeymap[KEY_EQUALS] = XK_equal;
    mLocalKeymap[KEY_APOSTROPHE] = XK_apostrophe;
    mLocalKeymap[KEY_COMMA] = XK_comma;
    mLocalKeymap[KEY_MINUS] = XK_minus;
    mLocalKeymap[KEY_PERIOD] = XK_period;
    mLocalKeymap[KEY_SLASH] = XK_slash;
    mLocalKeymap[KEY_LEFTBRACKET] = XK_bracketleft;
    mLocalKeymap[KEY_BACKSLASH] = XK_backslash;
    mLocalKeymap[KEY_RIGHTBRACKET] = XK_bracketright;
    mLocalKeymap[KEY_GRAVE_ACCENT] = XK_grave;

    // error!!!
    mLocalKeymap[KEY_ESCAPE] = XK_Escape;
    mLocalKeymap[KEY_RETURN] = XK_Return;
    mLocalKeymap[KEY_TAB] = XK_Tab;
    mLocalKeymap[KEY_BACKSPACE] = XK_BackSpace;
    mLocalKeymap[KEY_INSERT] = XK_Insert;
    mLocalKeymap[KEY_DELETE] = XK_Delete;
    mLocalKeymap[KEY_UP] = XK_Up;
    mLocalKeymap[KEY_DOWN] = XK_Down;
    mLocalKeymap[KEY_RIGHT] = XK_Right;
    mLocalKeymap[KEY_LEFT] = XK_Left;
    mLocalKeymap[KEY_PAGEUP] = XK_Page_Up;
    mLocalKeymap[KEY_PAGEDOWN] = XK_Page_Down;
    mLocalKeymap[KEY_HOME] = XK_Home;
    mLocalKeymap[KEY_END] = XK_End;
    mLocalKeymap[KEY_CAPSLOCK] = XK_Caps_Lock;
    mLocalKeymap[KEY_SCROLLOCK] = XK_Scroll_Lock;
    mLocalKeymap[KEY_NUMLOCK] = XK_Num_Lock;
    mLocalKeymap[KEY_PRINTSCREEN] = XK_Print;
    mLocalKeymap[KEY_PAUSE] = XK_Pause;

    mLocalKeymap[KEY_F1] = XK_F1;
    mLocalKeymap[KEY_F2] = XK_F2;
    mLocalKeymap[KEY_F3] = XK_F3;
    mLocalKeymap[KEY_F4] = XK_F4;
    mLocalKeymap[KEY_F5] = XK_F5;
    mLocalKeymap[KEY_F6] = XK_F6;
    mLocalKeymap[KEY_F7] = XK_F7;
    mLocalKeymap[KEY_F8] = XK_F8;
    mLocalKeymap[KEY_F9] = XK_F9;
    mLocalKeymap[KEY_F10] = XK_F10;
    mLocalKeymap[KEY_F11] = XK_F11;
    mLocalKeymap[KEY_F12] = XK_F12;

    mLocalKeymap[KEY_LSHIFT] = XK_Shift_L;
    mLocalKeymap[KEY_LCTRL] = XK_Control_L;
    mLocalKeymap[KEY_LALT] = XK_Alt_L;
    mLocalKeymap[KEY_LSUPER] = XK_Meta_L;
    mLocalKeymap[KEY_RSHIFT] = XK_Shift_R;
    mLocalKeymap[KEY_RCTRL] = XK_Control_R;
    mLocalKeymap[KEY_RALT] = XK_Alt_R;
    mLocalKeymap[KEY_RSUPER] = XK_Meta_R;
    mLocalKeymap[KEY_MENU] = XK_Menu;
    mLocalKeymap[KEY_HANGUL] = XK_Hangul;
    mLocalKeymap[KEY_HANJA] = XK_Hangul_Hanja;

    mLocalKeymap[KEY_KP_0] = XK_KP_0;
    mLocalKeymap[KEY_KP_1] = XK_KP_1;
    mLocalKeymap[KEY_KP_2] = XK_KP_2;
    mLocalKeymap[KEY_KP_3] = XK_KP_3;
    mLocalKeymap[KEY_KP_4] = XK_KP_4;
    mLocalKeymap[KEY_KP_5] = XK_KP_5;
    mLocalKeymap[KEY_KP_6] = XK_KP_6;
    mLocalKeymap[KEY_KP_7] = XK_KP_7;
    mLocalKeymap[KEY_KP_8] = XK_KP_8;
    mLocalKeymap[KEY_KP_9] = XK_KP_9;

    mLocalKeymap[KEY_KP_DECIMAL] = XK_KP_Decimal;
    mLocalKeymap[KEY_KP_DIVIDE] = XK_KP_Divide;
    mLocalKeymap[KEY_KP_MULTIPLY] = XK_KP_Multiply;
    mLocalKeymap[KEY_KP_SUBTRACT] = XK_minus;
    mLocalKeymap[KEY_KP_ADD] = XK_plus;
    mLocalKeymap[KEY_KP_ENTER] = XK_KP_Enter;
    mLocalKeymap[KEY_KP_EQUAL] = XK_KP_Equal;
  }

  Platform::~Platform() {
    LinuxPlatform::instance->KillPlatformLinux();
    SafeDealloc(LinuxPlatform::instance);
  }

  bool Platform::Initialize(const PlatformContext& param) {
    return LinuxPlatform::instance->CreatePlatformLinux(
        param.title, param.width, param.height, param.fullscreen, param.majorVersion,
        param.minorVersion, param.style, param.profile);
  }

  void Platform::Update() {
    XEvent event;
    mMousePosition.z = 0.f;

    while (XPending(LinuxPlatform::instance->mDisplay)) {
      bool isScroll = false;
      XNextEvent(LinuxPlatform::instance->mDisplay, &event);

      switch (event.type) {
      case Expose: {
        XExposeEvent& expose = event.xexpose;
        glViewport(0, 0, expose.width, expose.height);
        SwapBuffer();
        break;
      }
      case KeyPress:
        printf("KeyPress: %d\n", event.xkey.keycode);
        break;
      case KeyRelease: {
        break;
      }
      case ButtonPress:
      case ButtonRelease: {
        int button = MOUSE_UNDEFINED;

        switch (event.xbutton.button) {
        case Button1: // left click
          button = MOUSE_LEFT;
          break;
        case Button2: // middle click
          button = MOUSE_MIDDLE;
          break;
        case Button3: // right click
          button = MOUSE_RIGHT;
          break;
        case Button4: // scroll up
          mMousePosition.z += 1.f;
          isScroll = true;
          break;
        case Button5: // scroll down
          mMousePosition.z -= 1.f;
          isScroll = true;
          break;
        }

        if (!isScroll)
          mMouseButtons[button] = (event.type == ButtonPress);

        break;
      }
      case MotionNotify: // mouse position
        mMousePosition.x = static_cast<float>(event.xmotion.x);
        mMousePosition.y = static_cast<float>(event.xmotion.y);
        break;
      case ClientMessage:
        if (static_cast<unsigned long>(event.xclient.data.l[0]) ==
            LinuxPlatform::instance->mDestroyMessage) {
          mIsRunning = false;
        }
        break;
      }
    }
  }

  void Platform::SwapBuffer() {
    glXSwapBuffers(LinuxPlatform::instance->mDisplay, LinuxPlatform::instance->mWindow);
  }

  void Platform::WindowSizeChanged(int width, int height) {
    mWidth = width;
    mHeight = height;
    glViewport(0, 0, mWidth, mHeight);
  }

  const Point Platform::CenterOnWindow() {
    int w, h;
    GetDesktopSize(w, h);

    return Point((w - mWidth) / 2.f, (h - mHeight) / 2.f);
  }

  void Platform::GetDesktopSize(int& width, int& height) {
    Window rootWindow = DefaultRootWindow(LinuxPlatform::instance->mDisplay);
    XWindowAttributes attribs;
    XGetWindowAttributes(LinuxPlatform::instance->mDisplay, rootWindow, &attribs);

    width = attribs.width;
    height = attribs.height;
  }

  void Platform::SetMousePos(const int x, const int y) const {}

  int Platform::GetScreenDPI() {}

  void Platform::SetVSync(bool sync) {}

  int Platform::GetVSync() {}

  bool Platform::IsFocus() const {}
}

#endif /* Theodore_Linux */
