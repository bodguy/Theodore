#include "Component.h"
#include "../GameObject.h"
#include "Transform.h"

namespace Theodore {
  Component::Component(const std::string& name) : Object(name), mIsEnable(true) { mTransform = mGameObject->GetComponent<Transform>(); }

  Component::~Component() {}

  Transform* Component::GetTransform() const { return mTransform; }

  bool Component::IsEnabled() const { return mIsEnable; }

  void Component::SetEnabled(bool enable) { mIsEnable = enable; }

  MessageResult Component::HandleMessage(Message& msg) { return MessageResult::Ignored; }
}  // namespace Theodore