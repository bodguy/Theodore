#include "Input.h"
#include "Platform.h"

namespace Quark {
    Input* Input::instance = nullptr;
    Input::Input() {
        instance = this;
        
        for (int i = 0; i < KEY_MAX; i++) {
            mPreviousKeys[i] = mCurrentKeys[i] = false;
        }
        
        for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
            mPreviousMouseButtons[i] = mCurrentMouseButtons[i] = false;
        }
    }
    
    Input::~Input() {
        
    }
    
    void Input::Update() {
        for (int i = 0; i < KEY_MAX; i++) {
            mPreviousKeys[i] = mCurrentKeys[i];
            mCurrentKeys[i] = Platform::GetInstance()->mKeys[i];
        }
        
        for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
            mPreviousMouseButtons[i] = mCurrentMouseButtons[i];
            mCurrentMouseButtons[i] = Platform::GetInstance()->mMouseButtons[i];
        }
        
        mMouseDelta = Platform::GetInstance()->mMousePosition - mlastMousePos;
		mMouseDelta.z = 0.f; // mouse wheel(z position) must be always 0 for Magnitude calculation
        if (mMouseDelta.Magnitude() > 50.0f) { // threshold value is 50.0f
            // renew old mouse position then wait for next frame when entering this function again
            mlastMousePos = Platform::GetInstance()->mMousePosition;
            return;
        }
        mlastMousePos = Platform::GetInstance()->mMousePosition;
    }
    
    bool Input::GetKeyDown(KeyCode keyCode) {
        return instance->mCurrentKeys[keyCode] && !instance->mPreviousKeys[keyCode];
    }
    
    bool Input::GetKeyUp(KeyCode keyCode) {
        return !instance->mCurrentKeys[keyCode] && instance->mPreviousKeys[keyCode];
    }
    
    bool Input::GetKeyHeld(KeyCode keyCode) {
        return instance->mCurrentKeys[keyCode];
    }
    
    Vector3d Input::GetMousePosition() {
        return instance->mlastMousePos;
    }
    
    Vector3d Input::GetMouseDeltaPosition() {
        return instance->mMouseDelta;
    }
    
    bool Input::GetMouseButtonDown(MouseButton button) {
        return instance->mCurrentMouseButtons[button] && !instance->mPreviousMouseButtons[button];
    }
    
    bool Input::GetMouseButtonUp(MouseButton button) {
        return !instance->mCurrentMouseButtons[button] && instance->mPreviousMouseButtons[button];
    }
    
    bool Input::GetMouseButtonHeld(MouseButton button) {
        return instance->mCurrentMouseButtons[button];
    }
}
