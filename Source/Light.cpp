#include "Light.h"
#include "GameObject.h"

namespace Quark {
	Light::Light(LightType type) : Component("Light"), type(type), ambient(Color(0.2f, 0.2f, 0.2f, 0.2f)), diffuse(), specular(),
		intensity(0.f), constant(0.f), linear(0.f), quadratic(0.f), cutOff(0.f), outerCutOff(0.f) {
		switch (type) {
		case LightType::DirectionalLight:
			break;
		case LightType::PointLight:
			constant = 1.f;
			linear = 0.09f;
			quadratic = 0.032f;
			break;
		case LightType::SpotLight:
			intensity = 0.f;
			cutOff = 0.f;
			outerCutOff = 0.f;
			break;
		}
		mTransform = this->mGameObject->GetTransform();
	}

	Light::~Light() {
	}

	Transform* Light::GetTransform() const {
		return mTransform;
	}

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
