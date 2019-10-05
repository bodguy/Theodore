#include "Input.h"
#include "../Helper/Utility.h"
#include "../Math/Math.h"
#include "Platform.h"

namespace Theodore {
  InputHandler::InputHandler(KeyCode positive, KeyCode negative, float delta) : mPositive(positive), mNegative(negative), mAccumulator(0.f), mDeltaSpeed(delta) {}

  InputHandler::~InputHandler() {}

  KeyCode InputHandler::GetPositive() const { return mPositive; }

  KeyCode InputHandler::GetNegative() const { return mNegative; }

  float InputHandler::GetAccumulator() const { return mAccumulator; }

  void InputHandler::Accumulate(float delta) { mAccumulator = Math::Clamp(mAccumulator + delta, -1.f, 1.f); }

  void InputHandler::ResetAccumulator() { mAccumulator = 0.f; }

  float InputHandler::GetDelta() const { return mDeltaSpeed; }

  Input* Input::instance = nullptr;
  Input::Input() {
    instance = this;

    for (int i = 0; i < KEY_MAX; i++) {
      mPreviousKeys[i] = mCurrentKeys[i] = false;
    }

    for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
      mPreviousMouseButtons[i] = mCurrentMouseButtons[i] = false;
    }

    // default input handler
    mHandlerMapping.insert(std::make_pair(std::string("Horizontal"), new InputHandler(KEY_D, KEY_A, 0.01f)));
    mHandlerMapping.insert(std::make_pair(std::string("Vertical"), new InputHandler(KEY_W, KEY_S, 0.01f)));
  }

  Input::~Input() {
    for (auto& i : mHandlerMapping) {
      SafeDealloc(i.second);
    }
  }

  void Input::Update() {
    if (Platform::GetInstance()->IsFocus()) {
      for (int i = 0; i < KEY_MAX; i++) {
        mPreviousKeys[i] = mCurrentKeys[i];
        mCurrentKeys[i] = Platform::GetInstance()->mKeys[i];
      }

      for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
        mPreviousMouseButtons[i] = mCurrentMouseButtons[i];
        mCurrentMouseButtons[i] = Platform::GetInstance()->mMouseButtons[i];
      }

      mMouseDelta = Platform::GetInstance()->mMousePosition - mlastMousePos;
      mMouseDelta.z = 0.f;                 // mouse wheel(z position) must be always 0 for Magnitude calculation
      if (mMouseDelta.Length() > 50.0f) {  // threshold value is 50.0f
        // renew old mouse position then wait for next frame when entering this function again
        mlastMousePos = Platform::GetInstance()->mMousePosition;
        return;
      }
      mlastMousePos = Platform::GetInstance()->mMousePosition;
    } else {
      memset(mPreviousKeys, 0, KEY_MAX);
      memset(mCurrentKeys, 0, KEY_MAX);

      memset(mPreviousMouseButtons, 0, MOUSE_BUTTON_MAX);
      memset(mCurrentMouseButtons, 0, MOUSE_BUTTON_MAX);

      memset(&mMouseDelta, 0, sizeof(Vector3d));
      memset(&mlastMousePos, 0, sizeof(Vector3d));
    }
  }

  bool Input::GetKeyDown(KeyCode keyCode) { return instance->mCurrentKeys[keyCode] && !instance->mPreviousKeys[keyCode]; }

  bool Input::GetKeyUp(KeyCode keyCode) { return !instance->mCurrentKeys[keyCode] && instance->mPreviousKeys[keyCode]; }

  bool Input::GetKeyHeld(KeyCode keyCode) { return instance->mCurrentKeys[keyCode]; }

  Vector3d Input::GetMousePosition() { return instance->mlastMousePos; }

  Vector3d Input::GetMouseDeltaPosition() { return instance->mMouseDelta; }

  bool Input::GetMouseButtonDown(MouseButton button) { return instance->mCurrentMouseButtons[button] && !instance->mPreviousMouseButtons[button]; }

  bool Input::GetMouseButtonUp(MouseButton button) { return !instance->mCurrentMouseButtons[button] && instance->mPreviousMouseButtons[button]; }

  bool Input::GetMouseButtonHeld(MouseButton button) { return instance->mCurrentMouseButtons[button]; }

  bool Input::AddAxis(const std::string& axisName, InputHandler* handler) {
    if (instance->mHandlerMapping.find(axisName) != instance->mHandlerMapping.end()) return false;

    instance->mHandlerMapping.insert(std::make_pair(axisName, handler));
    return true;
  }

  float Input::GetAxis(const std::string& axis) {
    auto iter = instance->mHandlerMapping.find(axis);

    if (iter != instance->mHandlerMapping.end()) {
      InputHandler* input = iter->second;
      if (Input::GetKeyHeld(input->GetPositive())) {
        input->Accumulate(input->GetDelta());  // positive
      } else if (Input::GetKeyHeld(input->GetNegative())) {
        input->Accumulate(-1.f * input->GetDelta());  // negative
      } else {
        input->ResetAccumulator();
      }

      return input->GetAccumulator();
    }

    return 0.f;
  }

  float Input::GetAxisRaw(const std::string& axis) {
    auto iter = instance->mHandlerMapping.find(axis);

    if (iter != instance->mHandlerMapping.end()) {
      InputHandler* input = iter->second;
      if (Input::GetKeyHeld(input->GetPositive())) {
        return 1.f;  // positive
      } else if (Input::GetKeyHeld(input->GetNegative())) {
        return -1.f;  // negative
      }
    }

    return 0.f;
  }

  bool Input::AnyKeyDown() {
    bool isDown = false;

    for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
      isDown = Input::GetMouseButtonDown(static_cast<MouseButton>(i));
      if (isDown) return isDown;
    }

    for (int i = 0; i < KEY_MAX; i++) {
      isDown = Input::GetKeyDown(static_cast<KeyCode>(i));
      if (isDown) return isDown;
    }

    return isDown;
  }

  bool Input::AnyKeyUp() {
    bool isDown = false;

    for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
      isDown = Input::GetMouseButtonUp(static_cast<MouseButton>(i));
      if (isDown) return isDown;
    }

    for (int i = 0; i < KEY_MAX; i++) {
      isDown = Input::GetKeyUp(static_cast<KeyCode>(i));
      if (isDown) return isDown;
    }

    return isDown;
  }

  bool Input::AnyKeyHeld() {
    bool isDown = false;

    for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
      isDown = Input::GetMouseButtonHeld(static_cast<MouseButton>(i));
      if (isDown) return isDown;
    }

    for (int i = 0; i < KEY_MAX; i++) {
      isDown = Input::GetKeyHeld(static_cast<KeyCode>(i));
      if (isDown) return isDown;
    }

    return isDown;
  }
}  // namespace Theodore
