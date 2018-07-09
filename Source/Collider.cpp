#include "Collider.h"

namespace Quark {
	Collider::Collider(const std::string& name) : Component(name), mType(ColliderType::Undefined), mIsVisible(false), mColor(Color::green) {
	}

	Collider::~Collider() {
	}

	ColliderType Collider::GetType() const {
		return mType;
	}

	void Collider::SetVisible(bool isVisible) {
		mIsVisible = isVisible;
	}

	void Collider::SetColor(const Color& color) {
		mColor = color;
	}
}