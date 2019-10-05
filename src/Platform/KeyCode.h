/**
  @brief keycode constant
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef KeyCode_h
#define KeyCode_h

namespace Theodore {
  enum MouseButton { MOUSE_UNDEFINED = -1, MOUSE_LEFT = 0, MOUSE_RIGHT, MOUSE_MIDDLE, MOUSE_BUTTON_MAX };

  enum KeyCode {
    KEY_UNDEFINED = -1,
    /* Printable keys */
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SPACE,
    KEY_SEMICOLON,    /* ; */
    KEY_EQUALS,       /* = */
    KEY_APOSTROPHE,   /* ' */
    KEY_COMMA,        /* , */
    KEY_MINUS,        /* - */
    KEY_PERIOD,       /* . */
    KEY_SLASH,        /* / */
    KEY_LEFTBRACKET,  /* [ */
    KEY_BACKSLASH,    /* \ */
    KEY_RIGHTBRACKET, /* ] */
    KEY_GRAVE_ACCENT, /* ` */
    /* Function keys */
    KEY_ESCAPE, /* ESC */
    KEY_RETURN, /* ENTER */
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_UP,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPSLOCK,
    KEY_SCROLLOCK,
    KEY_NUMLOCK,
    KEY_PRINTSCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_LSHIFT,
    KEY_LCTRL,
    KEY_LALT,
    KEY_LSUPER, /* left shift, left ctrl, left alt, left super(aka, windows key) */
    KEY_RSHIFT,
    KEY_RCTRL,
    KEY_RALT,
    KEY_RSUPER, /* right shift, right ctrl, right alt, right super(aka, windows key) */
    KEY_MENU,
    KEY_HANGUL,
    KEY_HANJA,
    /* key pad */
    KEY_KP_0,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_DECIMAL,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_SUBTRACT,
    KEY_KP_ADD,
    KEY_KP_ENTER,
    KEY_KP_EQUAL,
    KEY_UNKNOWN1,
    KEY_UNKNOWN2,
    KEY_UNKNOWN3,
    KEY_UNKNOWN4,
    KEY_UNKNOWN5,
    KEY_UNKNOWN6,
    KEY_UNKNOWN7,
    KEY_UNKNOWN8,
    KEY_UNKNOWN9,
    KEY_UNKNOWN10,
    KEY_UNKNOWN11,
    KEY_UNKNOWN12,
    KEY_UNKNOWN13,
    KEY_UNKNOWN14,
    KEY_UNKNOWN15,
    KEY_UNKNOWN16,
    KEY_UNKNOWN17,
    KEY_UNKNOWN18,
    KEY_UNKNOWN19,
    KEY_UNKNOWN20,
    KEY_UNKNOWN21,
    KEY_MAX
  };
} // namespace Theodore
#endif /* KeyCode_h */
