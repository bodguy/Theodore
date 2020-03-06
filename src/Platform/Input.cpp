// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Input.h"

#include <cstring>

#include "Helper/Utility.h"
#include "Math/Mathf.h"
#include "Platform.h"

namespace Theodore {
  InputHandler::InputHandler(KeyCode positive, KeyCode negative, float delta) : positive(positive), negative(negative), accumulator(0.f), deltaSpeed(delta) {}

  InputHandler::~InputHandler() {}

  KeyCode InputHandler::GetPositive() const { return positive; }

  KeyCode InputHandler::GetNegative() const { return negative; }

  float InputHandler::GetAccumulator() const { return accumulator; }

  void InputHandler::Accumulate(float delta) { accumulator = Mathf::Clamp(accumulator + delta, -1.f, 1.f); }

  void InputHandler::ResetAccumulator() { accumulator = 0.f; }

  float InputHandler::GetDelta() const { return deltaSpeed; }

  Input* Input::instance = nullptr;
  Input::Input() {
    instance = this;

    for (int i = 0; i < KEY_MAX; i++) {
			previousKeys[i] = currentKeys[i] = false;
    }

    for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
			previousMouseButtons[i] = currentMouseButtons[i] = false;
    }

    // default input handler
    handlerMapping.insert(std::make_pair(std::string("Horizontal"), new InputHandler(KEY_D, KEY_A, 0.01f)));
    handlerMapping.insert(std::make_pair(std::string("Vertical"), new InputHandler(KEY_W, KEY_S, 0.01f)));
  }

  Input::~Input() {
    for (auto& i : handlerMapping) {
      SafeDealloc(i.second);
    }
  }

  void Input::Update() {
    if (Platform::GetInstance()->IsFocus()) {
      for (int i = 0; i < KEY_MAX; i++) {
				previousKeys[i] = currentKeys[i];
				currentKeys[i] = Platform::GetInstance()->keys[i];
      }

      for (int i = 0; i < MOUSE_BUTTON_MAX; i++) {
				previousMouseButtons[i] = currentMouseButtons[i];
				currentMouseButtons[i] = Platform::GetInstance()->mouseButtons[i];
      }

			mouseDeltaPosition = Platform::GetInstance()->mousePosition - lastMousePosition;
			lastMousePosition = Platform::GetInstance()->mousePosition;
      return;
    }

    memset(previousKeys, 0, KEY_MAX);
    memset(currentKeys, 0, KEY_MAX);

    memset(previousMouseButtons, 0, MOUSE_BUTTON_MAX);
    memset(currentMouseButtons, 0, MOUSE_BUTTON_MAX);

    memset(&mouseDeltaPosition, 0, sizeof(Vector3d));
    memset(&lastMousePosition, 0, sizeof(Vector3d));
  }

  bool Input::GetKeyDown(KeyCode keyCode) { return instance->currentKeys[keyCode] && !instance->previousKeys[keyCode]; }

  bool Input::GetKeyUp(KeyCode keyCode) { return !instance->currentKeys[keyCode] && instance->previousKeys[keyCode]; }

  bool Input::GetKeyHeld(KeyCode keyCode) { return instance->currentKeys[keyCode]; }

  Vector3d Input::GetMousePosition() { return instance->lastMousePosition; }

  Vector3d Input::GetMouseDeltaPosition() { return instance->mouseDeltaPosition; }

  bool Input::GetMouseButtonDown(MouseButton button) { return instance->currentMouseButtons[button] && !instance->previousMouseButtons[button]; }

  bool Input::GetMouseButtonUp(MouseButton button) { return !instance->currentMouseButtons[button] && instance->previousMouseButtons[button]; }

  bool Input::GetMouseButtonHeld(MouseButton button) { return instance->currentMouseButtons[button]; }

  bool Input::AddAxis(const std::string& axisName, InputHandler* handler) {
    if (instance->handlerMapping.find(axisName) != instance->handlerMapping.end()) return false;

    instance->handlerMapping.insert(std::make_pair(axisName, handler));
    return true;
  }

  float Input::GetAxis(const std::string& axis) {
    auto iter = instance->handlerMapping.find(axis);

    if (iter != instance->handlerMapping.end()) {
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
    auto iter = instance->handlerMapping.find(axis);

    if (iter != instance->handlerMapping.end()) {
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
