#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace Quark {
	Component::Component(const std::string& name) :Object(name) {
		mTransform = mGameObject->GetComponent<Transform>();
	}
	
	Component::~Component() {
	
	}

	Transform* Component::GetTransform() const {
		return mTransform;
	}

	MessageResult Component::HandleMessage(Message& msg) {
		return MessageResult::Ignored;
	}
}