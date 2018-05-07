#ifndef Light_h
#define Light_h

#include "Enumeration.h"
#include "Color.h"
#include "Vector3d.h"
#include "Component.h"

namespace Quark {
	class Light : public Component {
	public:
		Light();
		~Light();

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Vector3d mPosition;
		Vector3d mDirection;
		Color mColor;
		float mIntensity;
		float mConstant;
		float mLinear;
		float mQuadratic;
		float mCutOff;
		float mOuterCutOff;
		Enumeration::LightType mType;
	};
}

#endif /* Light_h */