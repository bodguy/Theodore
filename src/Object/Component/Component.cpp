// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Component.h"
#include "Object/GameObject.h"
#include "Transform.h"

namespace Theodore {
  Component::Component(const std::string& name) : Object(name), mIsEnable(true) { mTransform = mGameObject->GetComponent<Transform>(); }

  Component::~Component() {}

  Transform* Component::GetTransform() const { return mTransform; }

  bool Component::IsEnabled() const { return mIsEnable; }

  void Component::SetEnabled(bool enable) { mIsEnable = enable; }

  MessageResult Component::HandleMessage(Message& msg) { return MessageResult::Ignored; }
}  // namespace Theodore