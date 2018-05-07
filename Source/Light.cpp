#include "Light.h"

namespace Quark {
	Light::Light() : Component("Light"), mType(Enumeration::Unknown) {
	}

	Light::~Light() {
	}

	// private functions, for consistency with other components.

	void Light::Update(double deltaTime) {

	}

	void Light::Render() {
		return;
	}

	bool Light::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool Light::Destroy() {
		return false;
	}
}
