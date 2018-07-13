#ifndef Input_h
#define Input_h

#include "Vector3d.h"
#include "KeyCode.h"

namespace Theodore {
    class Input {
    public:
        Input();
        ~Input();
        
        void Update();
        
        static bool GetKeyDown(KeyCode keyCode);
        static bool GetKeyUp(KeyCode keyCode);
        static bool GetKeyHeld(KeyCode keyCode);
        
        static Vector3d GetMousePosition();
        static Vector3d GetMouseDeltaPosition();
        static bool GetMouseButtonDown(MouseButton button);
        static bool GetMouseButtonUp(MouseButton button);
        static bool GetMouseButtonHeld(MouseButton button);
        
    private:
        static Input* instance;
        
        bool mPreviousKeys[KEY_MAX];
        bool mCurrentKeys[KEY_MAX];
        
        bool mPreviousMouseButtons[MOUSE_BUTTON_MAX];
        bool mCurrentMouseButtons[MOUSE_BUTTON_MAX];
        
        Vector3d mlastMousePos;
        Vector3d mMouseDelta;
    };
}

#endif /* Input_h */
