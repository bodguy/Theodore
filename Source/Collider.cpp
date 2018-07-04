#include "Collider.h"

namespace Quark {
	Collider::Collider(const std::string& name) : Component(name), mType(ColliderType::Undefined) {
	}

	Collider::~Collider() {
	}

	ColliderType Collider::GetType() const {
		return mType;
	}
}