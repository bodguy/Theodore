#ifndef Light_h
#define Light_h

#include "Enumeration.h"
#include "Color.h"
#include "Component.h"

namespace Quark {
	class Transform;
	class Light : public Component {
	public:
		Light(LightType type);
		~Light();

		Transform* GetTransform() const;
		Color GetAmbient() const { return mAmbient; }
		Color GetDiffuse() const { return mDiffuse; }
		Color GetSpecular() const { return mSpecular; }

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		LightType mType;
		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;

		float mIntensity;
		float mConstant;
		float mLinear;
		float mQuadratic;
		float mCutOff;
		float mOuterCutOff;
		Transform* mTransform;
	};
}

#endif /* Light_h */