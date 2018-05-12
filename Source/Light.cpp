#include "Light.h"
#include "GameObject.h"

namespace Quark {
	Light::Light(LightType type) : Component("Light"), mType(type), mAmbient(), mDiffuse(), mSpecular() {
		mIntensity = 0.f;
		mConstant = 0.f;
		mLinear = 0.f;
		mQuadratic = 0.f;
		mCutOff = 0.f;
		mOuterCutOff = 0.f;
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
