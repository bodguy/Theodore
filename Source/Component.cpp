#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace Quark {
	Component::Component(const std::string& name) :Object(name), mIsEnable(true) {
		mTransform = mGameObject->GetComponent<Transform>();
	}
	
	Component::~Component() {
	
	}

	Transform* Component::GetTransform() const {
		return mTransform;
	}

	bool Component::IsEnable() const {
		return mIsEnable;
	}

	void Component::SetEnable(bool enable) {
		mIsEnable = enable;
	}

	MessageResult Component::HandleMessage(Message& msg) {
		return MessageResult::Ignored;
	}
}